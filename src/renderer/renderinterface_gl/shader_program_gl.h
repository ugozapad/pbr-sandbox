#ifndef SHADER_PROGRAM_GL_H
#define SHADER_PROGRAM_GL_H

#include "renderer/renderinterface_gl/renderinterface_gl.h"

class ShaderProgram_GL : public ShaderProgram
{
public:
	ShaderProgram_GL(const ShaderDescriptor& shaderDesc);
	~ShaderProgram_GL();

	void Init(const ShaderDescriptor& shaderDesc);
	void Shutdown();

	GLuint GetHandle();

private:
	GLuint m_program;

	uint32_t GetUniformLocation(const char* uniformName) override;
	void SetTextureSampler(int slot, const char* name) override;
	void SetInteger(int slot, int value) override;
	void SetFloat(int slot, float value) override;
	void SetVector3(int slot, const glm::vec3& vector) override;
	void SetMatrix4(int slot, const glm::mat4& matrix) override;
};

#endif // !SHADER_PROGRAM_GL_H
