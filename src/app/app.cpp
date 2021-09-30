#include "app/app.h"
#include "app/camera.h"
#include "app/resource_manager.h"

#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/shaderprogrammanager.h"
#include "render/shaderprogram.h"
#include "render/scene.h"
#include "render/constantscache.h"

#include "glad/glad.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#ifdef _MSC_VER
#include <spdlog/sinks/msvc_sink.h>
#endif // _MSC_VER

struct AppPrivate
{
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
	ShaderProgram* m_shader_prog;
	RenderDevice* m_renderDevice;
	Scene* m_sponzaScene;

	std::shared_ptr<Texture2D> m_texture;

	Camera m_camera;

	glm::mat4 m_proj;

	void create()
	{
		m_renderDevice = RenderDevice::getInstance();

		float vertices[] = {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		m_vertex_buffer = m_renderDevice->createVertexBuffer(vertices, sizeof(vertices), BufferAccess::Static);

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		m_index_buffer = m_renderDevice->createIndexBuffer(indices, sizeof(indices), BufferAccess::Static);

		m_shader_prog = ShaderProgramManager::getInstance().createProgram("test", "data/test.vsh", "data/test.psh");

		m_texture = ResourceManager::getInstance().createResource<Texture2D>("data/test.png");

		m_sponzaScene = Scene::createFromFile("data/sponza/glTF/Sponza.gltf");
	}

	void destroy()
	{
		delete m_sponzaScene;

		m_texture.reset();

		//ShaderProgramManager::get_instance().delete_program(m_shader_prog);
		m_renderDevice->deleteVertexBuffer(m_vertex_buffer);
	}

	void update(float dt)
	{
		updateCamera(dt);
	}

	void updateCamera(float dt)
	{
		GLFWwindow* window = App::getInstance().getWindow();

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

		updateMatrices();

		ShaderConstantCache::getInstance().setCameraPos(m_camera.m_pos);
	}

	void updateMatrices()
	{
		GLFWwindow* window = App::getInstance().getWindow();

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float aspect = (float)width / (float)height;
		m_proj = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 1000.0f);

		ShaderConstantCache::getInstance().setProj(m_proj);
		ShaderConstantCache::getInstance().setView(m_camera.get_view_matr());
	}

	void render()
	{
		m_renderDevice->clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH);

		glEnable(GL_DEPTH_TEST);

		//m_texture->bind(0);

		//m_render_device->setVertexBuffer(m_vertex_buffer);

		//// position attribute
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);

		//// color attribute
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		//// texture coord attribute
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		//m_render_device->setIndexBuffer(m_index_buffer);

		//ShaderProgramManager::getInstance().setShaderProgram(m_shader_prog);

		//// set matrices
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));

		//glm::mat4 mvp = glm::identity<glm::mat4>();
		//mvp = m_proj * m_camera.get_view_matr() * model;
		//m_shader_prog->setMatrix4("u_mvp", mvp);

		//m_render_device->drawElements(PM_TRIANGLES, 6);

		m_sponzaScene->draw();
	}

};

static AppPrivate g_app_private;

void APIENTRY R_GLDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{

	if (type != GL_DEBUG_TYPE_ERROR)
		return;

	if (type == GL_DEBUG_TYPE_ERROR)
		spdlog::error("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
			type, severity, message);
	else
		spdlog::info("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
			type, severity, message);

	if (type == GL_DEBUG_TYPE_ERROR)
		DebugBreak();
}

void App::init()
{
	spdlog::logger* logger = spdlog::default_logger_raw();
	logger->sinks().clear();
	logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("engine_log.txt", true));
	logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#ifdef _MSC_VER
	logger->sinks().push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif // _MSC_VER

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(1600, 900, "pbr-sandbox", nullptr, nullptr);
	glfwSetWindowPos(m_window, 0, 0);
	glfwMakeContextCurrent(m_window);

	gladLoadGL();

	ShaderProgramManager::getInstance().init();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(R_GLDebugOutput, 0);

	g_app_private.create();
}

void App::shutdown()
{
	g_app_private.destroy();

	ShaderProgramManager::getInstance().shutdown();

	glfwTerminate();
}

void App::run()
{
	RenderDevice* render_device = RenderDevice::getInstance();

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
	App::getInstance().init();
	App::getInstance().run();
	App::getInstance().shutdown();

	return 0;
}