#include "Camera.h"

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
void Camera::SetView(const glm::mat4 & view)
{
	m_view = view;
}

void Camera::SetPosition(const glm::vec3 & position)
{
	m_model = glm::translate(glm::mat4(1.0f), position);
}
void Camera::SetModel(const glm::mat4 & model)
{
	m_model = model;
}