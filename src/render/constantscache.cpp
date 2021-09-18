#include "render/constantscache.h"

void ShaderConstantCache::setModel(const glm::mat4& mat)
{
	m_modelMatrix = mat;
	m_inversedModelMatrix = glm::inverse(m_modelMatrix);
	m_normalMatrix = glm::transpose(m_inversedModelMatrix);
}
