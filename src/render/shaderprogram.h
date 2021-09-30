#pragma once

#include "render/render_common.h"

class ShaderProgramManager;
class Material;

class ShaderProgram
{
	friend class ShaderProgramManager;
	friend class Material;
private:
	ShaderProgram(const char* name, const char* vsfilename, const char* fsfilename);
	~ShaderProgram();
public:
	void setTextureSampler(int slot, const char* name);
	void setInteger(const char* uniformName, int value);
	void setFloat(const char* uniformName, float value);

	void setVector2(const char* uniform_name, const glm::vec2& vector);
	void setVector3(const char* uniform_name, const glm::vec3& vector);
	void setVector3(const char* uniform_name, float x, float y, float z);
	void setVector4(const char* uniform_name, const glm::vec4& vector);

	void setMatrix3(const char* uniform_name, const glm::mat3& matrix);
	void setMatrix3x4(const char* uniform_name, const glm::mat4x3& matrix);
	void setMatrix4(const char* uniform_name, const glm::mat4& matrix);
private:
	uint32_t getUniformLocation(const char* uniform_name);
private:
	const char* m_file_name;
	uint32_t m_program;
};