#include "render/light.h"

Light::Light(const glm::vec3& position)
{
	m_diffuseColor = glm::vec3(1.0f);
	m_position = position;
}

void LightController::addLight(Light* light)
{
	m_lights.push_back(light);
}
