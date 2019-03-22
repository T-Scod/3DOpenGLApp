#pragma once
#include "Application.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Shader.h"
#include "Mesh.h"

class App3D : public aie::Application
{
public:
	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};

public:
	App3D();
	~App3D();

	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();

	void RunApp();

protected:
	Camera* m_camera;

	aie::ShaderProgram m_shader;
	Mesh* m_mesh;
	glm::mat4* m_transform;
};