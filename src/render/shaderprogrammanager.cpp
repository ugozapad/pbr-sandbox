#include "render/shaderprogrammanager.h"
#include "render/shaderprogram.h"

#include "glad/glad.h"

void ShaderProgramManager::init()
{

}

void ShaderProgramManager::shutdown()
{
	for (std::vector<ShaderProgram*>::iterator it = m_programs.begin(); it != m_programs.end(); ++it)
		if (*it)
			delete_program(*it);

	m_programs.clear();
}

void ShaderProgramManager::set_shader_program(ShaderProgram* program)
{
	if (program)
		glUseProgram(program->m_program);
	else
		glUseProgram(0);
}

ShaderProgram* ShaderProgramManager::create_program(const char* name, const char* vsfilename, const char* fsfilename)
{
	ShaderProgram* program = new ShaderProgram(name, vsfilename, fsfilename);
	m_programs.push_back(program);
	return program;
}

void ShaderProgramManager::delete_program(ShaderProgram* program)
{
	if (program)
	{
		delete program;
	}
}
