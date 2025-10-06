#ifndef SHADER_PROGRAM_GL_H
#define SHADER_PROGRAM_GL_H

#include "renderer/render.h"

class ShaderProgram_GL
{
public:
	ShaderProgram_GL(const ShaderDescriptor& shaderDesc);
	~ShaderProgram_GL();

	void Init(const ShaderDescriptor& shaderDesc);
	void Shutdown();

	uint32_t GetUniformLocation(const char* uniformName);
	void SetTextureSampler(int slot, const char* name);
	void SetInteger(int slot, int value);
	void SetFloat(int slot, float value);
	void SetVector3(int slot, const glm::vec3& vector);
	void SetMatrix4(int slot, const glm::mat4& matrix);

	GLuint GetHandle();

private:
	GLuint m_program;
};

typedef ShaderProgram_GL ShaderProgram;

#endif // !SHADER_PROGRAM_GL_H
