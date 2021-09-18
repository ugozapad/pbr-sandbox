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
	m_shader_prog->setTextureSampler(0, "u_tex0");

}

void Material::render(size_t vertices_nbr, const glm::mat4& model)
{
	bind();

	m_shader_prog->setVector3("u_objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	m_shader_prog->setVector3("u_lightColor", glm::vec3(0.5f, 0.5f, 0.5f));
	m_shader_prog->setVector3("u_lightPos", glm::vec3(1.2f, 1.0f, 2.0f));

	ShaderConstantCache& cache = ShaderConstantCache::getInstance();

	m_shader_prog->setMatrix4("u_model", cache.getModel());
	m_shader_prog->setMatrix4("u_view", cache.getView());
	m_shader_prog->setMatrix4("u_projection", cache.getProj());

	glm::mat4 mvp = glm::identity<glm::mat4>();
	mvp = cache.getProj() * cache.getView() * model;

	m_shader_prog->setMatrix4("u_mvp", mvp);
	m_shader_prog->setMatrix4("u_inversedModelMatrix", cache.getInversedModelMatrix());
	m_shader_prog->setMatrix4("u_normalMatrix", cache.getNormalMatrix());
	m_shader_prog->setVector3("u_cameraPosition", cache.getCameraPos());

	RenderDevice::getInstance()->drawElements(PM_TRIANGLES, vertices_nbr);
}
