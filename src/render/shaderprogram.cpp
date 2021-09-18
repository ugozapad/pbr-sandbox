#include "render/shaderprogram.h"

#include "glad/glad.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <string>

GLuint create_shader(GLenum target, const char* filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		spdlog::error("create_shader: failed to open file {}", filename);

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
		spdlog::error("create_shader: failed to compile shader {}\n{}", filename, infoLog);
	}

	spdlog::info("created {} shader from file {}", (target == GL_VERTEX_SHADER) ? "vertex" : "fragment", filename);

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
		spdlog::info("ShaderProgram::ShaderProgram: failed to link program {}", infoLog);
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

void ShaderProgram::setTextureSampler(int slot, const char* name)
{
	glUniform1i(get_uniform_location(name), slot);
}

void ShaderProgram::set_vector2(const char* uniform_name, const glm::vec2& vector)
{
	glUniform2fv(get_uniform_location(uniform_name), 1, glm::value_ptr(vector));
}

void ShaderProgram::setVector3(const char* uniform_name, const glm::vec3& vector)
{
	glUniform3fv(get_uniform_location(uniform_name), 1, glm::value_ptr(vector));
}

void ShaderProgram::set_vector4(const char* uniform_name, const glm::vec4& vector)
{
	glUniform4fv(get_uniform_location(uniform_name), 1, glm::value_ptr(vector));
}

void ShaderProgram::set_matrix3(const char* uniform_name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::set_matrix3x4(const char* uniform_name, const glm::mat4x3& matrix)
{
	glUniformMatrix3x4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setMatrix4(const char* uniform_name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix));
}

uint32_t ShaderProgram::get_uniform_location(const char* uniform_name)
{
	return glGetUniformLocation(m_program, uniform_name);
}
