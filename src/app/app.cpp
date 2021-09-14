#include "app/app.h"
#include "app/camera.h"
#include "app/resource_manager.h"

#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/shaderprogrammanager.h"
#include "render/shaderprogram.h"

#include "glad/glad.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

struct AppPrivate
{
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
	ShaderProgram* m_shader_prog;
	RenderDevice* m_render_device;

	std::shared_ptr<Texture2D> m_texture;

	Camera m_camera;

	glm::mat4 m_proj;

	tinygltf::Model m_sponza_model;

	void create()
	{
		m_render_device = RenderDevice::get_instance();

		float vertices[] = {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		m_vertex_buffer = m_render_device->create_vertex_buffer(vertices, sizeof(vertices), BufferAccess::Static);

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		m_index_buffer = m_render_device->create_index_buffer(indices, sizeof(indices), BufferAccess::Static);

		m_shader_prog = ShaderProgramManager::get_instance().create_program("test", "data/test.vsh", "data/test.psh");

		m_texture = ResourceManager::get_instance().create_resource<Texture2D>("data/test.png");

		// load scene
		std::string err;
		std::string warn;
		tinygltf::TinyGLTF loader;
		bool ret = loader.LoadASCIIFromFile(&m_sponza_model, &err, &warn, "data/sponza/glTF/Sponza.gltf");
		if (!warn.empty()) {
			spdlog::error("Warn: {}", warn);
		}

		if (!err.empty()) {
			spdlog::error("Err: {}", err);
		}

		if (!ret) {
			spdlog::error("Failed to parse glTF");
		}
	}

	void destroy()
	{
		m_texture.reset();

		//ShaderProgramManager::get_instance().delete_program(m_shader_prog);
		m_render_device->delete_vertex_buffer(m_vertex_buffer);
	}

	void update(float dt)
	{
		update_camera(dt);
	}

	void update_camera(float dt)
	{
		GLFWwindow* window = App::get_instance().get_window();

		Camera::MovmentDir camDir = (Camera::MovmentDir)0;
		if (glfwGetKey(window, GLFW_KEY_W))
			camDir = Camera::Forward;
		if (glfwGetKey(window, GLFW_KEY_S))
			camDir = Camera::Backward;
		if (glfwGetKey(window, GLFW_KEY_A))
			camDir = Camera::Left;
		if (glfwGetKey(window, GLFW_KEY_D))
			camDir = Camera::Right;

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		m_camera.update(camDir, x, y, width, height, dt);

		update_matrices();
	}

	void update_matrices()
	{
		GLFWwindow* window = App::get_instance().get_window();

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float aspect = (float)width / (float)height;
		m_proj = glm::perspective(glm::radians(75.0f), aspect, 0.001f, 1000.0f);
	}

	void render()
	{
		m_render_device->clear_color(0.5f, 0.5f, 0.5f, 1.0f);
		m_render_device->clear(RenderDevice::CLEAR_COLOR);

		m_texture->bind(0);

		m_render_device->set_vertex_buffer(m_vertex_buffer);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		m_render_device->set_index_buffer(m_index_buffer);

		ShaderProgramManager::get_instance().set_shader_program(m_shader_prog);

		// set matrices
		glm::mat4 world = glm::mat4(1.0f);

		glm::mat4 mvp = glm::identity<glm::mat4>();
		mvp = m_proj * m_camera.get_view_matr() * world;
		m_shader_prog->set_matrix4("u_mvp", mvp);

		m_render_device->draw_elements(PM_TRIANGLES, 6);

		draw_model(m_sponza_model);
	}

	void draw_model(tinygltf::Model& model)
	{

	}
};
	
static AppPrivate g_app_private;

void App::init()
{
	spdlog::logger* logger = spdlog::default_logger_raw();
	logger->sinks().clear();
	logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("engine_log.txt", true));
	logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(1280, 720, "pbr-sandbox", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	gladLoadGL();

	ShaderProgramManager::get_instance().init();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	g_app_private.create();
}

void App::shutdown()
{
	g_app_private.destroy();

	ShaderProgramManager::get_instance().shutdown();

	glfwTerminate();
}

void App::run()
{
	RenderDevice* render_device = RenderDevice::get_instance();

	float startTime = glfwGetTime();
	float endTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window))
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
			break;

		glfwSwapBuffers(m_window);
		glfwPollEvents();

		startTime = glfwGetTime();
		float dt = startTime - endTime;

		g_app_private.update(dt);

		g_app_private.render();

		endTime = startTime;
	}
}

int main()
{
	App::get_instance().init();
	App::get_instance().run();
	App::get_instance().shutdown();

	return 0;
}