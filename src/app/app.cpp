#include "app/app.h"
#include "render/renderdevice.h"
#include "glad/glad.h"

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