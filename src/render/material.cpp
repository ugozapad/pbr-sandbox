#include "render/material.h"

#include "app/resource_manager.h"

void Material::init(const MaterialCreationInfo& info)
{
	ResourceManager& resmgr = ResourceManager::get_instance();

	if (info.m_albedo_filename)
		m_tex_albedo = resmgr.create_resource<Texture2D>(info.m_albedo_filename);

	if (info.m_normal_filename)
		m_tex_normal = resmgr.create_resource<Texture2D>(info.m_normal_filename);

	m_clamp_edge = info.m_is_clamp_edge;
}

void Material::release()
{
	if (m_tex_normal)
		m_tex_normal.reset();

	if (m_tex_albedo)
		m_tex_albedo.reset();
}
