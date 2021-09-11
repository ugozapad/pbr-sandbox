#pragma once

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
};