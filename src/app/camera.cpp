#include "app/camera.h"

Camera::Camera()
{
	m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_yaw = -90.0f;
	m_pitch = 0.0f;
}

Camera::~Camera()
{

}

void Camera::update(MovmentDir dir, float x, float y, int width, int height, float dt)
{
	static float lastX = (float)width / 2.0;
	static float lastY = (float)height / 2.0;

	const float sensitivity = 0.3f;

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
	lastX = x;
	lastY = y;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	m_dir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_dir.y = sin(glm::radians(m_pitch));
	m_dir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_dir);

	float speed = 10.0f * dt;

	if (dir == Camera::Forward)
		m_pos += speed * m_front;
	if (dir == Camera::Backward)
		m_pos -= speed * m_front;
	if (dir == Camera::Left)
		m_pos -= glm::normalize(glm::cross(m_front, m_up)) * speed;
	if (dir == Camera::Right)
		m_pos += glm::normalize(glm::cross(m_front, m_up)) * speed;
}

glm::mat4 Camera::get_view_matr()
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}


