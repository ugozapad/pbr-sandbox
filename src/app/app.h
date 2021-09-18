#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class App
{
public:
	static App& getInstance() 
	{
		static App s_app;
		return s_app;
	}
public:
	void init();
	void shutdown();

	void run();

	GLFWwindow* getWindow() { return m_window; }

private:
	GLFWwindow* m_window;
};