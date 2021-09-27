#include "render/constantscache.h"

#include <glm/common.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>


void ShaderConstantCache::setModel(const glm::mat4& mat)
{
	m_modelMatrix = mat;
	m_inversedModelMatrix = glm::inverse(m_modelMatrix);

	m_normalMatrix = glm::transpose(glm::adjugate(glm::mat3(mat)));
}
