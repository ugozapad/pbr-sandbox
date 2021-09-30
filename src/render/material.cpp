#include "render/material.h"
#include "render/shaderprogram.h"
#include "render/shaderprogrammanager.h"
#include "render/constantscache.h"
#include "render/renderdevice.h"

#include "render/glad/include/glad/glad.h"

#include "app/resource_manager.h"

#include <spdlog/spdlog.h>

void Material::init(const MaterialCreationInfo& info)
{
	ResourceManager& resmgr = ResourceManager::getInstance();
	if (info.m_albedo_filename)
		m_tex_albedo = resmgr.createResource<Texture2D>(info.m_albedo_filename);

	if (info.m_normal_filename)
		m_tex_normal = resmgr.createResource<Texture2D>(info.m_normal_filename);

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

	if (m_tex_normal) {
		m_tex_normal->bind(1);
		m_shaderProgram->setTextureSampler(1, "u_normalSampler");
	}

	//uint32_t uniformBlockLocation = glGetUniformBlockIndex(m_shaderProgram->m_program, "PointLight");
	//spdlog::info("{}", uniformBlockLocation);
}

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

	static RenderDevice& device = *RenderDevice::getInstance();
	device.drawElements(PM_TRIANGLES, vertices_nbr);

	unbindDeviceStateAfterRender();
}

void Material::unbindDeviceStateAfterRender()
{
	for (int i = 0; i < 6; i++) {
		Texture2D::unbind(i);
	}
}
