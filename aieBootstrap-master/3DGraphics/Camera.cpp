/*
	\file Camera.cpp
	\brief A file of definitions for a camera class
*/
#include "Camera.h"
#include <GLFW/glfw3.h>

/*
	\var float m_scroll
	A globally stored value used for changing the field of view.
*/
float m_scroll = 0.0f;

/*
	\fn Camera()
	\brief Default constructor that initialises all member matrices to identity matrices.
	\fn Camera(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
	\brief Sets the corresponding member variable to the specified value.
	\param model The model matrix of the camera.
	\param view The view matrix of the camera.
	\param projection The projection matrix of the camera.
	\fn ~Camera()
	\brief Default destructor.
*/
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

/*
	\fn void Update(const float deltaTime)
	\brief Takes in user input to change the camera's position, rotation and field of view.
	\param deltaTime The time between each frame.
*/
void Camera::Update(const float deltaTime)
{
	// gets the current window
	GLFWwindow* window = glfwGetCurrentContext();
	// the vector that the camera will be translated by
	glm::vec3 position(0.0f);
	// gets the forward, right and up vectors from the local transformation, i.e. the view matrix.
	glm::vec3 forward = { -GetView()[0][2], -GetView()[1][2], -GetView()[2][2] };
	glm::vec3 up = { GetView()[0][1], GetView()[1][1], GetView()[2][1] };
	glm::vec3 right = { GetView()[0][0], GetView()[1][0], GetView()[2][0] };
	// the movement speed of the camera.
	float speed = 3.0f;
	// checks if the up or w key was pressed and increments the translation vector along the forward vector
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += forward * speed * deltaTime;
	}
	// checks if the down or s key was pressed and decrements the translation vector along the forward vector
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= forward * speed * deltaTime;
	}
	// checks if the right or d key was pressed and increments the translation vector along the right vector
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * speed * deltaTime;
	}
	// checks if the left or a key was pressed and decrements the translation vector along the right vector
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right * speed * deltaTime;
	}
	// checks if the e key was pressed and increments the translation vector along the up vector
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * speed * deltaTime;
	}
	// checks if the q key was pressed and decrements the translation vector along the up vector
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * speed * deltaTime;
	}

	// moves the camera by the translation vector
	Translate(position);

	// the speed of the mouse
	float mouseSpeed = 0.01f;
	// used to store the position of the mouse
	double xPos, yPos;
	// gets the mouse position
	glfwGetCursorPos(window, &xPos, &yPos);
	// used to store the dimensions of the window
	int width, height;
	// gets the dimensions of the window
	glfwGetWindowSize(window, &width, &height);
	// sets the position of the mouse to the middle of the window
	glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
	// calulates the direction the mouse is moving based on its displacement from the middle of the screen
	glm::vec2 mouseDir = { ((double)width / 2.0) - xPos, ((double)height / 2.0) - yPos };

	// rotates the camera based on the mouse's x movement around a global up axis
	Rotate(mouseDir.x * mouseSpeed * deltaTime, GetView() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	// rotates the camera based on the mouse's y movement around a local right axis
	Rotate(mouseDir.y * mouseSpeed * deltaTime, { 1.0f, 0.0f, 0.0f });

	// the default field of view of the camera
	float FoV = 45.0f;
	// uses the scroll callback method to get the scroll wheel input
	glfwSetScrollCallback(window, ScrollCallback);
	// alters the FoV based on the scroll amount
	FoV = FoV - 5.0f * GetScroll();

	// sets the perspective of the camera using the new FoV
	Perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);

	window = nullptr;
}

/*
	\fn void Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far)
	\brief Uses the glm perspective function to set the perspective of the camera.
	\param fieldOfView The angle of the field of view of the camera.
	\param aspectRatio The aspect ratio of the camera.
	\param near The closest range that objects are rendered at.
	\param far The furthest range that objects are rendered at.
*/
void Camera::Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far)
{
	m_projection = glm::perspective(fieldOfView, aspectRatio, near, far);
}
/*
	\fn void SetProjection(const glm::mat4& projection)
	\brief Sets the projection matrix.
	\param projection The projection value.
*/
void Camera::SetProjection(const glm::mat4 & projection)
{
	m_projection = projection;
}

/*
	\fn void LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up)
	\brief Uses the glm lookAt function to make the camera look at a specific location.
	\param from The position of the camera.
	\param to The position the camera is looking at.
	\param up The local y axis.
*/
void Camera::LookAt(const glm::vec3 & from, const glm::vec3 & to, const glm::vec3 & up)
{
	m_view = glm::lookAt(from, to, up);
	// ensures the model matrix is always the inverse of the view matrix
	m_model = glm::inverse(m_view);
}
/*
	\fn void Rotate(const float angle, const glm::vec3& axis)
	\brief Uses the glm rotate function to rotate the camera around a particular axis by a certain amount.
	\param angle The amount the camera is rotating around by in radians.
	\param axis The axis the camera is rotating around.
*/
void Camera::Rotate(const float angle, const glm::vec3 & axis)
{
	m_model = glm::rotate(m_model, angle, axis);
	// ensures the view matrix is always the inverse of the model matrix
	m_view = glm::inverse(m_model);
}
/*
	\fn void SetView(const glm::mat4& view)
	\brief Sets the view matrix.
	\param view The view value.
*/
void Camera::SetView(const glm::mat4 & view)
{
	m_view = view;
	// ensures the model matrix is always the inverse of the view matrix
	m_model = glm::inverse(m_view);
}

/*
	\fn void Translate(const glm::vec3& position)
	\brief Translates the camera by incrementing the current position by the vector.
	\param vector The direction and magnitude of the translation.
*/
void Camera::Translate(const glm::vec3 & vector)
{
	m_model[3][0] += vector.x;
	m_model[3][1] += vector.y;
	m_model[3][2] += vector.z;
	// ensures the view matrix is always the inverse of the model matrix
	m_view = glm::inverse(m_model);
}
/*
	\fn void SetModel(const glm::mat4& model)
	\brief Sets the model matrix.
	\param model The model value.
*/
void Camera::SetModel(const glm::mat4 & model)
{
	m_model = model;
	// ensures the view matrix is always the inverse of the model matrix
	m_view = glm::inverse(m_model);
}

/*
	\fn static void ScrollCallback(GLFWwindow* window, double x, double y)
	\brief Increments a global variable that is used for changing the field of view via the scroll wheel.
	\param window The application window.
	\param x The x component of the scroll wheel.
	\param y The component of the scroll wheel.
*/
void Camera::ScrollCallback(GLFWwindow * window, double x, double y)
{
	m_scroll += (float)y;
}
/*
	\fn float GetScroll() const
	\brief Gets the globally stored scroll amount.
	\return Returns the scroll amount.
*/
float Camera::GetScroll() const
{
	return m_scroll;
}