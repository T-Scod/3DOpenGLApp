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

class RenderingApp : public aie::Application
{
public:
	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};

public:
	RenderingApp();
	~RenderingApp();

	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();

	void RunApp();
	template <typename T>
	void SetLightUniform(aie::ShaderProgram shader, const char* propertyName, size_t lightIndex, const T& value)
	{
		std::ostringstream ss;
		ss << "allLights[" << lightIndex << "]." << propertyName;
		std::string uniformName = ss.str();

		shader.bindUniform(uniformName.c_str(), value);
	}

protected:
	/*struct Light
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};*/

	struct Light
	{
		glm::vec3 position;
		glm::vec3 intensities;
		float attenuation;
		float ambientCoefficient;
		float coneAngle;
		glm::vec3 coneDirection;
	};

	Camera* m_camera;
	//aie::ShaderProgram m_simpleShader;
	//aie::ShaderProgram m_textureShader;
	//aie::ShaderProgram m_spearShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalShader;

	//Mesh* m_mesh;
	//glm::mat4* m_transform;

	//aie::OBJMesh m_objMesh;
	//glm::mat4 m_objTransform;

	//Mesh* m_quadMesh;
	//aie::Texture m_gridTexture;

	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	Light m_light;
	std::vector<Light> m_lights;
	glm::vec3 m_ambientLight;
};