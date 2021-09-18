#pragma once

#include "render/render_common.h"

#include <vector>

class Light
{
public:
	Light(const glm::vec3& position);

public:
	glm::vec3 m_diffuseColor;
	glm::vec3 m_position;
};

class LightController
{
public:
	static LightController& getInstance() {
		static LightController instance;
		return instance;
	}

public:
	void addLight(Light* light);

	std::vector<Light*>& getLights() { return m_lights; }

private:
	std::vector<Light*> m_lights;
};