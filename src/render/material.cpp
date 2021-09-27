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

	m_shaderProgram = ShaderProgramManager::getInstance().createProgram("model", "data/model.vsh", "data/model.psh");
}

void Material::release()
{

}

void Material::bind()
{
	ShaderProgramManager::getInstance().setShaderProgram(m_shaderProgram);

	m_tex_albedo->bind(0);
	m_shaderProgram->setTextureSampler(0, "u_albedoSampler");

}

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

void Material::render(size_t vertices_nbr, const glm::mat4& model)
{
	bind();

	ShaderConstantCache& cache = ShaderConstantCache::getInstance();

	m_shaderProgram->setMatrix4("u_model", cache.getModel());
	m_shaderProgram->setMatrix4("u_view", cache.getView());
	m_shaderProgram->setMatrix4("u_projection", cache.getProj());

	glm::mat4 mvp = glm::identity<glm::mat4>();
	mvp = cache.getProj() * cache.getView() * model;

	m_shaderProgram->setMatrix4("u_mvp", mvp);
	m_shaderProgram->setMatrix4("u_inversedModelMatrix", cache.getInversedModelMatrix());
	m_shaderProgram->setMatrix3("u_normalMatrix", cache.getNormalMatrix());
	m_shaderProgram->setVector3("u_cameraPosition", cache.getCameraPos());

	RenderDevice::getInstance()->drawElements(PM_TRIANGLES, vertices_nbr);
}
