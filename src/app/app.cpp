#include "app/app.h"
#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/shaderprogrammanager.h"
#include "render/shaderprogram.h"

#include "glad/glad.h"

struct Scene
{
	VertexBuffer* m_vertex_buffer;
	ShaderProgram* m_shader_prog;
	RenderDevice* m_render_device;

	void create()
	{
		m_render_device = RenderDevice::get_instance();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		m_vertex_buffer = m_render_device->create_vertex_buffer(vertices, sizeof(vertices), BufferAccess::Static);
		m_shader_prog = ShaderProgramManager::get_instance().create_program("test", "data/test.vsh", "data/test.psh");
	}

	void destroy()
	{
		//ShaderProgramManager::get_instance().delete_program(m_shader_prog);
		m_render_device->delete_vertex_buffer(m_vertex_buffer);
	}

	void update(float dt)
	{

	}

	void render()
	{
		m_render_device->clear_color(0.5f, 0.5f, 0.5f, 1.0f);
		m_render_device->clear(RenderDevice::CLEAR_COLOR);

		m_render_device->set_vertex_buffer(g_scene.m_vertex_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		ShaderProgramManager::get_instance().set_shader_program(g_scene.m_shader_prog);

		m_render_device->draw_arrays(PM_TRIANGLES, 0, 3);
	}
};

static Scene g_scene;

void App::init()
{
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

	g_scene.create();
}

void App::shutdown()
{
	g_scene.destroy();

	ShaderProgramManager::get_instance().shutdown();

	glfwTerminate();
}

void App::run()
{
	RenderDevice* render_device = RenderDevice::get_instance();

	while (!glfwWindowShouldClose(m_window))
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
			break;

		glfwSwapBuffers(m_window);
		glfwPollEvents();

		g_scene.render();
	}
}

int main()
{
	App::get_instance().init();
	App::get_instance().run();
	App::get_instance().shutdown();

	return 0;
}