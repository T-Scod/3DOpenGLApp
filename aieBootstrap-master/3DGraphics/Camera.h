/*
	\file Camera.h
	\brief A file of declarations for a camera class
*/
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// forward declared structure for access to GLFW window
struct GLFWwindow;

/*
	\class Camera
	\brief A user controlled camera for looking around a scene.
*/
class Camera
{
public:
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
	Camera();
	Camera(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	~Camera();

	/*
		\fn void Update(const float deltaTime)
		\brief Takes in user input to move the camera around.
		\param deltaTime The time between each frame.
	*/
	void Update(const float deltaTime);
	/*
		\fn glm::mat4 GetProjectionView() const
		\brief Calculates and returns the projection view matrix.
		\return Returns the projection view matrix.
	*/
	glm::mat4 GetProjectionView() const { return (m_projection * m_view); }

	/*
		\fn void Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far)
		\brief Sets the perspective of the camera.
		\param fieldOfView The angle of the field of view of the camera.
		\param aspectRatio The aspect ratio of the camera.
		\param near The closest range that objects are rendered at.
		\param far The furthest range that objects are rendered at.
	*/
	void Perspective(const float fieldOfView, const float aspectRatio, const float near, const float far);
	/*
		\fn void SetProjection(const glm::mat4& projection)
		\brief Sets the projection matrix.
		\param projection The projection value.
	*/
	void SetProjection(const glm::mat4& projection);
	/*
		\fn glm::mat4 GetProjection() const
		\brief Gets the projection matrix.
		\return Returns the projection matrix.
	*/
	glm::mat4 GetProjection() const { return m_projection; }
	
	/*
		\fn void LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up)
		\brief Makes the camera look at a specific location.
		\param from The position of the camera.
		\param to The position the camera is looking at.
		\param up The local y axis.
	*/
	void LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up);
	/*
		\fn void Rotate(const float angle, const glm::vec3& axis)
		\brief Rotates the camera around a particular axis by a certain amount.
		\param angle The amount the camera is rotating around by in radians.
		\param axis The axis the camera is rotating around.
	*/
	void Rotate(const float angle, const glm::vec3& axis);
	/*
		\fn void SetView(const glm::mat4& view)
		\brief Sets the view matrix.
		\param view The view value.
	*/
	void SetView(const glm::mat4& view);
	/*
		\fn glm::mat4 GetView() const
		\brief Gets the view matrix.
		\return Returns the view matrix.
	*/
	glm::mat4 GetView() const { return m_view; }

	/*
		\fn void Translate(const glm::vec3& position)
		\brief Translates the camera by the vector.
		\param vector The direction and magnitude of the translation.
	*/
	void Translate(const glm::vec3& vector);
	/*
		\fn void SetModel(const glm::mat4& model)
		\brief Sets the model matrix.
		\param model The model value.
	*/
	void SetModel(const glm::mat4& model);
	/*
		\fn glm::mat4 GetModel() const
		\brief Gets the model matrix.
		\return Returns the model matrix.
	*/
	glm::mat4 GetModel() const { return m_model; }

	/*
		\fn static void ScrollCallback(GLFWwindow* window, double x, double y)
		\brief Increments a global variable that is used for changing the field of view via the scroll wheel.
		\param window The application window.
		\param x The x component of the scroll wheel.
		\param y The component of the scroll wheel.
	*/
	static void ScrollCallback(GLFWwindow* window, double x, double y);
	/*
		\fn float GetScroll() const
		\brief Gets the globally stored scroll amount.
		\return Returns the scroll amount.
	*/
	float GetScroll() const;

protected:
	/*
		\var glm::mat4 m_model
		The model matrix of the camera, i.e. the world transform.
		\var glm::mat4 m_view
		The view matrix of the camera which is used to give perspective to objects.
		\var glm::mat4 m_projection
		The projection matrix of the camera which clips the area that is seen by the camera.
	*/
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
};