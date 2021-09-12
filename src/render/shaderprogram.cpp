#include "render/shaderprogram.h"

#include "glad/glad.h"

#include <fstream>
#include <string>

GLuint create_shader(GLenum target, const char* filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		printf("create_shader: failed to open file %s\n", filename);

	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	const char* contentCStr = content.c_str();

	GLuint shader = glCreateShader(target);
	glShaderSource(shader, 1, &contentCStr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("create_shader: failed to compile shader %s\n%s\n", filename, infoLog);
	}

	return shader;
}

ShaderProgram::ShaderProgram(const char* name, const char* vsfilename, const char* fsfilename)
{
	m_file_name = strdup(name);

	GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vsfilename);
	GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fsfilename);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertex_shader);
	glAttachShader(m_program, fragment_shader);
	glLinkProgram(m_program);

	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		printf("ShaderProgram::ShaderProgram: failed to link program %s\n", infoLog);
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}
