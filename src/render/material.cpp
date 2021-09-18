#include "render/material.h"
#include "render/shaderprogram.h"
#include "render/shaderprogrammanager.h"
#include "render/constantscache.h"
#include "render/renderdevice.h"

#include "app/resource_manager.h"

void Material::init(const MaterialCreationInfo& info)
{
	ResourceManager& resmgr = ResourceManager::getInstance();
	if (info.m_albedo_filename)
		m_tex_albedo = resmgr.createResource<Texture2D>(info.m_albedo_filename);

	//if (info.m_normal_filename)
	//	m_tex_normal = resmgr.create_resource<Texture2D>(info.m_normal_filename);

	m_clamp_edge = info.m_clamp_edge;
	m_transparent = info.m_transparent;

	m_shader_prog = ShaderProgramManager::getInstance().createProgram("model", "data/model.vsh", "data/model.psh");
}

void Material::release()
{

}

void Material::bind()
{
	ShaderProgramManager::getInstance().setShaderProgram(m_shader_prog);

	m_tex_albedo->bind(0);
	m_shader_prog->set_texture_sampler(0, "u_tex0");

}

void Material::render(size_t vertices_nbr, const glm::mat4& model)
{
	bind();

	ShaderConstantCache& cache = ShaderConstantCache::getInstance();

	glm::mat4 mvp = glm::identity<glm::mat4>();
	mvp = cache.getProj() * cache.getView() * model;

	m_shader_prog->set_matrix4("u_mvp", mvp);

	RenderDevice::getInstance()->drawElements(PM_TRIANGLES, vertices_nbr);
}
