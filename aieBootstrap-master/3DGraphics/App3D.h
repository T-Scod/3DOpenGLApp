#pragma once
#include "Application.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
	static void ScrollCallback(GLFWwindow* window, double x, double y);

protected:
	Camera* m_camera;

	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];
};