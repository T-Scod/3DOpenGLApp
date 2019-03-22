#include "Camera.h"
//#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

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

void Camera::Update(const float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();
	glm::vec3 position(0.0f);
	glm::vec3 forward = { -GetView()[0][2], -GetView()[1][2], -GetView()[2][2] };
	glm::vec3 up = { GetView()[0][1], GetView()[1][1], GetView()[2][1] };
	glm::vec3 right = { GetView()[0][0], GetView()[1][0], GetView()[2][0] };
	float speed = 3.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += forward * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= forward * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * speed * deltaTime;
	}

	Translate(position);

	float mouseSpeed = 0.01f;
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
	glm::vec2 mouseDir = { ((double)width / 2.0) - xPos, ((double)height / 2.0) - yPos };

	Rotate(mouseDir.x * mouseSpeed * deltaTime, GetView() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Rotate(mouseDir.y * mouseSpeed * deltaTime, { 1.0f, 0.0f, 0.0f });

	float initialFoV = 45.0f;
	glfwSetScrollCallback(window, ScrollCallback);
	initialFoV = initialFoV - 5.0f * GetScroll();

	Perspective(glm::radians(initialFoV), (float)width / (float)height, 0.1f, 100.0f);

	window = nullptr;
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