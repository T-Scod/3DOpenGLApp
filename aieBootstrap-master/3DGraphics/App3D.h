/*
	\file App3D.h
	\brief A file of declarations for an application class.
*/
#pragma once
#include "Application.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include <sstream>

/*
	\class App3D
	\brief Derives from the aie Application class but implements a custom start up function.
*/
class App3D : public aie::Application
{
public:
	/*
		\fn App3D()
		\brief Default constructor.
		\fn ~App3D()
		\brief Default destructor.
	*/
	App3D();
	~App3D();

	/*
		\fn bool startup()
		\brief Loads all the shaders and models as well as initialising the light sources.
		\return Returns false if something was not loaded properly.
	*/
	virtual bool startup();
	/*
		\fn void shutdown()
		\brief Deallocates all pointers.
	*/
	virtual void shutdown();
	/*
		\fn void update(float deltaTime)
		\brief Updates the camera each frame.
		\param deltaTime The time between each frame.
	*/
	virtual void update(float deltaTime);
	/*
		/fn void draw()
		/brief Draws all objects.
	*/
	virtual void draw();

	/*
		\fn RunApp()
		\brief Sets up the window and starts the game loop.
	*/
	void RunApp();

	/*
		\fn void SetLightUniform(aie::ShaderProgram* shader, const char* propertyName, size_t lightIndex, const T& value)
		\brief This function is templated with type name T.
		\brief Binds the uniform of the specified light property.
		\param shader The shader that the uniform is being bound for.
		\param propertyName The name of the shader light property that will be bound.
		\param lightIndex The index of the light in the array of lights.
		\tparam value The value that the bound uniform is being set to.
	*/
	template <typename T>
	void SetLightUniform(aie::ShaderProgram* shader, const char* propertyName, size_t lightIndex, const T& value)
	{
		// the string stream that will become the name of the uniform being bound to
		std::ostringstream ss;
		// concatenates the light index and property name to the uniform name
		ss << "allLights[" << lightIndex << "]." << propertyName;
		std::string uniformName = ss.str();

		// binds the uniform and sets its value
		shader->bindUniform(uniformName.c_str(), value);
	}

protected:
	/*
		\struct Light
		\brief An object that emits light.
		\var glm::vec4 position
		The position of the light.
		\var glm::vec3 Ia
		The ambient colour of the light.
		\var glm::vec3 Id
		The diffuse colour of the light.
		\var glm::vec3 Is
		The specular colour of the light.
		\var float attenuation
		The reduction of the intensity of the light over distance.
	*/
	struct Light
	{
		glm::vec4 position;
		glm::vec3 Ia;
		glm::vec3 Id;
		glm::vec3 Is;
		float attenuation;
	};

	/*
		\var Camera* m_camera
		The camera in the scene.
		\var aie::ShaderProgram m_phongShader
		The shader used to render the soul spear.
		\var aie::ShaderProgram m_simpleShader
		The shader used to render the light objects.
		\var aie::OBJMesh m_spearMesh
		The mesh of the soul spear.
		\var glm::mat4 m_spearTransform
		The transform of the soul spear.
		\var Mesh m_mesh
		The mesh of the light objects.
		\var std::vector<Light> m_lights
		A collection of the lights in the application.
	*/
	Camera* m_camera;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_simpleShader;
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;
	Mesh* m_mesh;
	std::vector<Light> m_lights;
};