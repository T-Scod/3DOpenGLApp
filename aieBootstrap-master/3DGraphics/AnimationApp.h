#pragma once
#include "Application.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class AnimationApp : public aie::Application
{
public:
	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};

public:
	AnimationApp();
	~AnimationApp();

	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();

	void RunApp();

protected:
	Camera* m_camera;

	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;
};