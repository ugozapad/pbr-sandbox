#pragma once

#include "render/render_common.h"

#include <vector>

#define MAX_LIGHTS 128

struct PointLight
{
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_color = glm::vec3(0.0f);
	//float m_intensity = 1.0f;
	//float m_radius = 1.0f;
};