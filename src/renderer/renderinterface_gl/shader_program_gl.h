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
};

#endif // !SHADER_PROGRAM_GL_H
