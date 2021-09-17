#pragma once

#include "render/texture2d.h"

#include <memory>

class ShaderProgram;

struct MaterialCreationInfo
{
	const char* m_albedo_filename;
	const char* m_normal_filename;
	bool m_clamp_edge;
	bool m_transparent;

	void init()
	{
		m_albedo_filename = nullptr;
		m_normal_filename = nullptr;
		m_clamp_edge = false;
		m_transparent = false;
	}
};

class Material
{
public:
	void init(const MaterialCreationInfo& info);
	void release();

	void bind();

	void render(size_t vertices_nbr, const glm::mat4& model);

private:
	std::shared_ptr<Texture2D> m_tex_albedo;
	std::shared_ptr<Texture2D> m_tex_normal;

	ShaderProgram* m_shader_prog;

	bool m_clamp_edge;
	bool m_transparent;
};