#pragma once

#include "render/render_common.h"

class ShaderProgramManager;

class ShaderProgram
{
	friend class ShaderProgramManager;
private:
	ShaderProgram(const char* name, const char* vsfilename, const char* fsfilename);
	~ShaderProgram();

private:
	const char* m_file_name;
	uint32_t m_program;
};