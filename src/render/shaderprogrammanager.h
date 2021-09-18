#pragma once

#include "render/render_common.h"

#include <vector>

class ShaderProgram;

class ShaderProgramManager
{
public:
	static ShaderProgramManager& getInstance()
	{
		static ShaderProgramManager instance;
		return instance;
	}
public:
	void init();
	void shutdown();

	void setShaderProgram(ShaderProgram* program);

	ShaderProgram* createProgram(const char* name, const char* vsfilename, const char* fsfilename);
	void delete_program(ShaderProgram* program);

private:
	std::vector<ShaderProgram*> m_programs;
};