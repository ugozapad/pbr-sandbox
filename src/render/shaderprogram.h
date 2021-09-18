#pragma once

#include "render/render_common.h"

class ShaderProgramManager;

class ShaderProgram
{
	friend class ShaderProgramManager;
private:
	ShaderProgram(const char* name, const char* vsfilename, const char* fsfilename);
	~ShaderProgram();
public:
	void setTextureSampler(int slot, const char* name);

	void set_vector2(const char* uniform_name, const glm::vec2& vector);
	void setVector3(const char* uniform_name, const glm::vec3& vector);
	void set_vector4(const char* uniform_name, const glm::vec4& vector);

	void set_matrix3(const char* uniform_name, const glm::mat3& matrix);
	void set_matrix3x4(const char* uniform_name, const glm::mat4x3& matrix);
	void setMatrix4(const char* uniform_name, const glm::mat4& matrix);
private:
	uint32_t get_uniform_location(const char* uniform_name);
private:
	const char* m_file_name;
	uint32_t m_program;
};