#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) :
		m_model(model), m_view(view), m_projection(projection) {}
	~Camera() {}

	virtual void Update(float deltaTime) = 0;
	void SetPerspective(const float fieldOfView, const float aspectRatio, const float near, const float far)
	{
		m_projection = glm::perspective(fieldOfView, aspectRatio, near, far);
	}
	void SetLookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up)
	{
		m_view = glm::lookAt(from, to, up);
	}
	

protected:
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
};