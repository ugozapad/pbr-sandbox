#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class App
{
public:
	static App& get_instance() 
	{
		static App s_app;
		return s_app;
	}
public:
	void init();
	void shutdown();

	void run();

private:
	GLFWwindow* m_window;
};