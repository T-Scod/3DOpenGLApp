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

class App3D : public aie::Application
{
public:
	App3D();
	~App3D();

	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();

	void RunApp();
	// binds the uniform of the specified light property
	template <typename T>
	void SetLightUniform(aie::ShaderProgram* shader, const char* propertyName, size_t lightIndex, const T& value)
	{
		std::ostringstream ss;
		// concatenates the light index and property name to the uniform name
		ss << "allLights[" << lightIndex << "]." << propertyName;
		std::string uniformName = ss.str();

		shader->bindUniform(uniformName.c_str(), value);
	}

protected:
	struct Light
	{
		glm::vec4 position;
		glm::vec3 Ia;
		glm::vec3 Id;
		glm::vec3 Is;
		float attenuation;
	};

	Camera* m_camera;

	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_simpleShader;

	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	Mesh* m_mesh;

	std::vector<Light> m_lights;
};