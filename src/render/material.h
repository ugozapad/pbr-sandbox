#pragma once

#include "render/texture2d.h"

#include <memory>

struct MaterialCreationInfo
{
	const char* m_albedo_filename;
	const char* m_normal_filename;
	bool m_is_clamp_edge;
};

class Material
{
public:
	void init(const MaterialCreationInfo& info);
	void release();

private:
	std::shared_ptr<Texture2D> m_tex_albedo;
	std::shared_ptr<Texture2D> m_tex_normal;

	bool m_clamp_edge;
};