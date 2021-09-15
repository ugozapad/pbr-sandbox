#pragma once

#include "render/render_common.h"

class ShaderConstantCache
{
public:
	static ShaderConstantCache& get_instance() {
		static ShaderConstantCache instance;
		return instance;
	}
public:
	void set_proj(const glm::mat4& mat) { m_proj = mat; }
	glm::mat4 get_proj() { return m_proj; }

	void set_view(const glm::mat4& mat) { m_view = mat; }
	glm::mat4 get_view() { return m_view; }

	void set_model(const glm::mat4& mat) { m_model = mat; }
	glm::mat4 get_model() { return m_model; }

private:
	glm::mat4 m_proj;
	glm::mat4 m_view;
	glm::mat4 m_model;
};