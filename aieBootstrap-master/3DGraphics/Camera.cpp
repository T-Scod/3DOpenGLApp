#include "Camera.h"

float m_scroll = 0.0f;

Camera::Camera()
{
	m_model = glm::mat4(1.0f);
	m_view = glm::mat4(1.0f);
	m_projection = glm::mat4(1.0f);
}
Camera::Camera(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection)
{
	m_model = model;
	m_view = view;
	m_projection = projection;
}
Camera::~Camera()
{
}

void Camera::Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far)
{
	m_projection = glm::perspective(fieldOfView, aspectRatio, near, far);
}
void Camera::SetProjection(const glm::mat4 & projection)
{
	m_projection = projection;
}

void Camera::LookAt(const glm::vec3 & from, const glm::vec3 & to, const glm::vec3 & up)
{
	m_view = glm::lookAt(from, to, up);
	m_model = glm::inverse(m_view);
}
void Camera::Rotate(const float angle, const glm::vec3 & axis)
{
	m_model = glm::rotate(m_model, angle, axis);
	m_view = glm::inverse(m_model);
}
void Camera::SetView(const glm::mat4 & view)
{
	m_view = view;
}

void Camera::Translate(const glm::vec3 & position)
{
	m_model[3][0] += position.x;
	m_model[3][1] += position.y;
	m_model[3][2] += position.z;
	m_view = glm::inverse(m_model);
}
void Camera::SetModel(const glm::mat4 & model)
{
	m_model = model;
}

void Camera::ScrollCallback(GLFWwindow * window, double x, double y)
{
	m_scroll += (float)y;
}

float Camera::GetScroll() const
{
	return m_scroll;
}