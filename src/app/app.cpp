#include "app/app.h"
#include "render/renderdevice.h"
#include "render/vertexbuffer.h"

#include "glad/glad.h"

struct Scene
{
	VertexBuffer* m_vertex_buffer;
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

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	g_scene.m_vertex_buffer = RenderDevice::get_instance()->create_vertex_buffer(vertices, sizeof(vertices), BufferAccess::Static);
}

void App::shutdown()
{
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

		render_device->clear_color(0.5f, 0.5f, 0.5f, 1.0f);
		render_device->clear(RenderDevice::CLEAR_COLOR);
	}
}

int main()
{
	App::get_instance().init();
	App::get_instance().run();
	App::get_instance().shutdown();

	return 0;
}