#include "render/shaderprogrammanager.h"
#include "render/shaderprogram.h"

#include "glad/glad.h"

#include <spdlog/spdlog.h>

void ShaderProgramManager::init()
{
	int maxVertexUniformBlocks = 0;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
	spdlog::info("max vertex uniform blocks {}", maxVertexUniformBlocks);
}

void ShaderProgramManager::shutdown()
{
	for (std::vector<ShaderProgram*>::iterator it = m_programs.begin(); it != m_programs.end(); ++it)
		if (*it)
			deleteProgram(*it);

	m_programs.clear();
}

void ShaderProgramManager::setShaderProgram(ShaderProgram* program)
{
	if (program)
		glUseProgram(program->m_program);
	else
		glUseProgram(0);
}

ShaderProgram* ShaderProgramManager::createProgram(const char* name, const char* vsfilename, const char* fsfilename)
{
	for (std::vector<ShaderProgram*>::iterator it = m_programs.begin(); it != m_programs.end(); ++it) {
		if (strcmp((*it)->m_file_name, name) == 0)
			return (*it);
	}

	ShaderProgram* program = new ShaderProgram(name, vsfilename, fsfilename);
	m_programs.push_back(program);
	return program;
}

void ShaderProgramManager::deleteProgram(ShaderProgram* program)
{
	if (program) {
		delete program;
	}
}
