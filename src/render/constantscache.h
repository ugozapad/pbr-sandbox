#pragma once

#include "render/render_common.h"

class ShaderConstantCache
{
public:
	static ShaderConstantCache& getInstance() {
		static ShaderConstantCache instance;
		return instance;
	}
public:
	void setProj(const glm::mat4& mat) { m_proj = mat; }
	glm::mat4 getProj() { return m_proj; }

	void setView(const glm::mat4& mat) { m_view = mat; }
	glm::mat4 getView() { return m_view; }

	void setModel(const glm::mat4& mat);
	glm::mat4 getModel() { return m_modelMatrix; }
	glm::mat4 getInversedModelMatrix() { return m_inversedModelMatrix; }
	glm::mat4 getNormalMatrix() { return m_normalMatrix; }

	void setCameraPos(const glm::vec3& pos) { m_cam_pos = pos; }
	glm::vec3 getCameraPos() { return m_cam_pos; }

private:
	glm::mat4 m_proj;
	glm::mat4 m_view;

	//////////////////////////
	// MODEL
	glm::mat4 m_modelMatrix;
	glm::mat4 m_inversedModelMatrix;
	glm::mat4 m_normalMatrix;

	glm::vec3 m_cam_pos;
};