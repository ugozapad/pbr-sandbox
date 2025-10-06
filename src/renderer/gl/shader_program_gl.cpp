#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include <fstream>
#include <string>
#include "core/filemanager.h"
#include "shader_program_gl.h"
#include <glm/gtc/type_ptr.hpp>

GLuint CreateShader(GLenum target, const char* filename, const char* defines = nullptr)
{
	std::string content;

	File* f = g_fileManager->OpenFile(filename, FileAccess::Read);
	SDL_assert(f->GetHandle());

	f->Seek(SeekDir::End, 0);
	size_t fileLength = f->Tell();
	f->Seek(SeekDir::Begin, 0);

	content.resize(fileLength + 1);
	f->Read((void*)content.data(), fileLength);
	content[fileLength] = '\0';

	g_fileManager->CloseFile(f);

	if (defines && strlen(defines) > 0) {
		size_t versionLocation = content.find("#version 330 core");

		size_t definesStringLength = versionLocation + strlen("#version 330 core") + 2;
		std::string newContentString = content.substr(0, definesStringLength);

		newContentString.append(defines);
		newContentString.append(content.substr(definesStringLength));

		content = newContentString;
	}

	const char* contentCStr = content.c_str();

	GLuint shader = glCreateShader(target);
	glShaderSource(shader, 1, &contentCStr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("=== SHADER COMPILE ERROR ===\n");
		printf("%s\n", contentCStr);
		printf("============================\n");
		printf("createShader: failed to compile shader %s\n%s\n", filename, infoLog);
	}

	printf("created %s shader from file %s\n", (target == GL_VERTEX_SHADER) ? "vertex" : "fragment", filename);

	return shader;
}

GLuint CreateShaderRaw(GLenum target, const ShaderDescriptor& shaderDesc, const char* defines)
{
	std::string content;

	switch (target)
	{
	case GL_VERTEX_SHADER:
		content = (const char*)shaderDesc.vertexData;
		break;

	case GL_FRAGMENT_SHADER:
		content = (const char*)shaderDesc.pixelData;
		break;

	default:
		SDL_assert(0 && "Dumb dumb dumb!!!");
		break;
	}

	if (defines && strlen(defines) > 0) {
		size_t versionLocation = content.find("#version 330 core");

		size_t definesStringLength = versionLocation + strlen("#version 330 core") + 2;
		std::string newContentString = content.substr(0, definesStringLength);

		newContentString.append(defines);
		newContentString.append(content.substr(definesStringLength));

		content = newContentString;
	}

	const char* contentCStr = content.c_str();

	GLuint shader = glCreateShader(target);
	glShaderSource(shader, 1, &contentCStr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("=== SHADER COMPILE ERROR ===\n");
		printf("%s\n", contentCStr);
		printf("============================\n");
		printf("createShader: failed to compile shader\n%s\n", infoLog);
	}

	return shader;
}

ShaderProgram_GL::ShaderProgram_GL(const ShaderDescriptor& shaderDesc) :
	m_program(-1)
{
	Init(shaderDesc);
}

ShaderProgram_GL::~ShaderProgram_GL()
{
	if (m_program >= 0)
		Shutdown();
}

void ShaderProgram_GL::Init(const ShaderDescriptor& shaderDesc)
{
	GLuint vertex_shader = CreateShaderRaw(GL_VERTEX_SHADER, shaderDesc, nullptr);
	GLuint fragment_shader = CreateShaderRaw(GL_FRAGMENT_SHADER, shaderDesc, nullptr);

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
		SDL_TriggerBreakpoint();
	}
}

void ShaderProgram_GL::Shutdown()
{
	glDeleteProgram(m_program);
}

GLuint ShaderProgram_GL::GetHandle()
{
	return m_program;
}

int32_t ShaderProgram_GL::GetUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(m_program, uniformName);
}

void ShaderProgram_GL::SetTextureSampler(int slot, const char* name)
{
	glUniform1i(GetUniformLocation(name), slot);
}

void ShaderProgram_GL::SetInteger(int slot, int value)
{
	glUniform1i(slot, value);
}

void ShaderProgram_GL::SetFloat(int slot, float value)
{
	glUniform1f(slot, value);
}

void ShaderProgram_GL::SetVector3(int slot, const glm::vec3& vector)
{
	glUniform3fv(slot, 1, glm::value_ptr(vector));
}

void ShaderProgram_GL::SetMatrix4(int slot, const glm::mat4& matrix)
{
	glUniformMatrix4fv(slot, 1, GL_FALSE, glm::value_ptr(matrix));
}
