#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// forward declared structure for access to GLFW window
struct GLFWwindow;

class Camera
{
public:
	Camera();
	Camera(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	~Camera();

	void Update(const float deltaTime);
	glm::mat4 GetProjectionView() const { return (m_projection * m_view); }

	void Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far);
	void SetProjection(const glm::mat4& projection);
	glm::mat4 GetProjection() const { return m_projection; }

	void LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up);
	void Rotate(const float angle, const glm::vec3& axis);
	void SetView(const glm::mat4& view);
	glm::mat4 GetView() const { return m_view; }

	void Translate(const glm::vec3& position);
	void SetModel(const glm::mat4& model);
	glm::mat4 GetModel() const { return m_model; }

	static void ScrollCallback(GLFWwindow* window, double x, double y);
	float GetScroll() const;

protected:
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
};