#pragma once

#include "render/render_common.h"

#include <vector>

class ShaderProgram;

class ShaderProgramManager
{
public:
	static ShaderProgramManager& get_instance()
	{
		static ShaderProgramManager instance;
		return instance;
	}
public:
	void init();
	void shutdown();

	void set_shader_program(ShaderProgram* program);

	ShaderProgram* create_program(const char* name, const char* vsfilename, const char* fsfilename);
	void delete_program(ShaderProgram* program);

private:
	std::vector<ShaderProgram*> m_programs;
};