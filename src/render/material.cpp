#include "render/material.h"
#include "render/shaderprogram.h"
#include "render/shaderprogrammanager.h"
#include "render/constantscache.h"
#include "render/renderdevice.h"

#include "app/resource_manager.h"

void Material::init(const MaterialCreationInfo& info)
{
	ResourceManager& resmgr = ResourceManager::get_instance();
	if (info.m_albedo_filename)
		m_tex_albedo = resmgr.create_resource<Texture2D>(info.m_albedo_filename);

	//if (info.m_normal_filename)
	//	m_tex_normal = resmgr.create_resource<Texture2D>(info.m_normal_filename);

	m_clamp_edge = info.m_clamp_edge;
	m_transparent = info.m_transparent;

	m_shader_prog = ShaderProgramManager::get_instance().create_program("model", "data/model.vsh", "data/model.psh");
}

void Material::release()
{
	if (m_tex_normal)
		m_tex_normal.reset();

	if (m_tex_albedo)
		m_tex_albedo.reset();
}

void Material::bind()
{
	ShaderProgramManager::get_instance().set_shader_program(m_shader_prog);

	m_tex_albedo->bind(0);
	m_shader_prog->set_texture_sampler(0, "u_tex0");

}

void Material::render(size_t vertices_nbr, const glm::mat4& model)
{
	bind();

	ShaderConstantCache& cache = ShaderConstantCache::get_instance();

	glm::mat4 mvp = glm::identity<glm::mat4>();
	mvp = cache.get_proj() * cache.get_view() * model;

	m_shader_prog->set_matrix4("u_mvp", mvp);

	RenderDevice::get_instance()->draw_elements(PM_TRIANGLES, vertices_nbr);
}
