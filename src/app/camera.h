#pragma once

#include "render/render_common.h"

class Camera
{
public:
	enum MovmentDir
	{
		None,
		Forward,
		Backward,
		Left,
		Right
	};
public:
	Camera();
	~Camera();

	void update(MovmentDir dir, float x, float y, int width, int height, float dt);

	glm::mat4 get_view_matr();

	glm::vec3 get_pos() { return m_pos; }
	void set_pos(glm::vec3& pos) { m_pos = pos; }
public:
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_dir;

	float m_yaw;
	float m_pitch;
};