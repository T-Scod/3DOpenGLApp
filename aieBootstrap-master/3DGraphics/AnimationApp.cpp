#include "AnimationApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>
#include "BoundingSphere.h"

AnimationApp::AnimationApp()
{
}
AnimationApp::~AnimationApp()
{
}

bool AnimationApp::startup()
{
	m_camera = new Camera();
	m_camera->LookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->Perspective(glm::pi<float>() * 0.25f, 1280.0f / 720.0f, 0.1f, 100.0f);

	m_positions[0] = glm::vec3(10.0f, 5.0f, 10.0f);
	m_positions[1] = glm::vec3(-10.0f, 0.0f, -10.0f);
	m_rotations[0] = glm::quat(glm::vec3(0.0f, -1.0f, 0.0f));
	m_rotations[1] = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f));

	m_hipFrames[0].position = glm::vec3(0.0f, 5.0f, 0.0f);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1.0f, 0.0f, 0.0f));
	m_hipFrames[1].position = glm::vec3(0.0f, 5.0f, 0.0f);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1.0f, 0.0f, 0.0f));
	m_kneeFrames[0].position = glm::vec3(0.0f, -2.5f, 0.0f);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1.0f, 0.0f, 0.0f));
	m_kneeFrames[1].position = glm::vec3(0.0f, -2.5f, 0.0f);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	m_ankleFrames[0].position = glm::vec3(0.0f, -2.5f, 0.0f);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(-1.0f, 0.0f, 0.0f));
	m_ankleFrames[1].position = glm::vec3(0.0f, -2.5f, 0.0f);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	return true;
}
void AnimationApp::shutdown()
{
	delete m_camera;
	m_camera = nullptr;
}

void AnimationApp::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_gameOver = true;
	}

	glm::vec3 position(0.0f);
	glm::vec3 forward = { -m_camera->GetView()[0][2], -m_camera->GetView()[1][2], -m_camera->GetView()[2][2] };
	glm::vec3 up = { m_camera->GetView()[0][1], m_camera->GetView()[1][1], m_camera->GetView()[2][1] };
	glm::vec3 right = { m_camera->GetView()[0][0], m_camera->GetView()[1][0], m_camera->GetView()[2][0] };
	float speed = 3.0f;
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += forward * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= forward * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * speed * deltaTime;
	}

	m_camera->Translate(position);

	float mouseSpeed = 0.01f;
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	glfwSetCursorPos(m_window, (double)width / 2.0, (double)height / 2.0);
	glm::vec2 mouseDir = { ((double)width / 2.0) - xPos, ((double)height / 2.0) - yPos };

	m_camera->Rotate(mouseDir.x * mouseSpeed * deltaTime, m_camera->GetView() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	m_camera->Rotate(mouseDir.y * mouseSpeed * deltaTime, { 1.0f, 0.0f, 0.0f });

	static float initialFoV = 45.0f;
	glfwSetScrollCallback(m_window, m_camera->ScrollCallback);
	float FoV = initialFoV - 5.0f * m_camera->GetScroll();

	m_camera->Perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);

	// animate leg
	float s = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 p = (1.0f - s) * m_hipFrames[0].position + s * m_hipFrames[1].position;
	// spherically interpolate hip rotation
	glm::quat r = glm::slerp(m_hipFrames[0].rotation, m_hipFrames[1].rotation, s);
	// update the hip bone
	m_hipBone = glm::translate(p) * glm::toMat4(r);

	s = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate knee position
	p = (1.0f - s) * m_kneeFrames[0].position + s * m_kneeFrames[1].position;
	// spherically interpolate knee rotation
	r = glm::slerp(m_kneeFrames[0].rotation, m_kneeFrames[1].rotation, s);
	// update the knee bone
	m_kneeBone = glm::translate(p) * glm::toMat4(r);
	// concatenates the knee bone with the hip bone
	m_kneeBone = m_hipBone * m_kneeBone;

	s = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate ankle position
	p = (1.0f - s) * m_ankleFrames[0].position + s * m_ankleFrames[1].position;
	// spherically interpolate ankle rotation
	r = glm::slerp(m_ankleFrames[0].rotation, m_ankleFrames[1].rotation, s);
	// update the ankle bone
	m_ankleBone = glm::translate(p) * glm::toMat4(r);
	// concatenates the ankle bone with the knee bone
	m_ankleBone = m_kneeBone * m_ankleBone;
}
void AnimationApp::draw()
{
	aie::Gizmos::addTransform(glm::mat4(1.0f));

	glm::vec4 white(1.0f);
	glm::vec4 black(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 21; i++)
	{
		aie::Gizmos::addLine(glm::vec3(-10.0f + i, 0.0f, 10.0f),
			glm::vec3(-10.0f + i, 0.0f, -10.0f),
			(i == 10) ? white : black);

		aie::Gizmos::addLine(glm::vec3(10.0f, 0.0f, -10.0f + i),
			glm::vec3(-10.0f, 0.0f, -10.0f + i),
			(i == 10) ? white : black);
	}

	glm::vec3 half(0.5f);
	// use time to animate a value between [0, 1]
	float s = glm::cos(getTime()) * 0.5f + 0.5f;
	// standard linear interpolation
	glm::vec3 p = (1.0f - s) * m_positions[0] + s * m_positions[1];
	// quaternion slerp
	glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	// build a matrix
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	// draw a transform and box
	aie::Gizmos::addTransform(m);
	aie::Gizmos::addAABBFilled(p, half, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), &m);

	glm::vec3 hipPos = glm::vec3(m_hipBone[3][0], m_hipBone[3][1], m_hipBone[3][2]);
	glm::vec3 kneePos = glm::vec3(m_kneeBone[3][0], m_kneeBone[3][1], m_kneeBone[3][2]);
	glm::vec3 anklePos = glm::vec3(m_ankleBone[3][0], m_ankleBone[3][1], m_ankleBone[3][2]);
	glm::vec4 pink(1.0f, 0.0f, 1.0f, 1.0f);

	aie::Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	aie::Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	aie::Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);

	BoundingSphere sphere;
	sphere.centre = glm::vec3(0.0f, cosf(getTime()) + 1.0f, 0.0f);
	sphere.radius = 0.5f;

	glm::vec4 plane(0.0f, 1.0f, 0.0f, 1.0f);

	float d = glm::dot(glm::vec3(plane), sphere.centre - plane.w);
	aie::Gizmos::addSphere(sphere.centre, sphere.radius, 8, 8, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	glm::vec4 planeColour(1.0f, 1.0, 0.0f, 1.0f);
	if (d > sphere.radius)
	{
		planeColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (d < -sphere.radius)
	{
		planeColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	aie::Gizmos::addTri(glm::vec3(4.0f, 1.0f, 4.0f), glm::vec3(-4.0f, 1.0f, -4.0f), glm::vec3(-4.0f, 1.0f, 4.0f), planeColour);
	aie::Gizmos::addTri(glm::vec3(4.0f, 1.0f, 4.0f), glm::vec3(4.0f, 1.0f, -4.0f), glm::vec3(-4.0f, 1.0f, -4.0f), planeColour);

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void AnimationApp::RunApp()
{
	// initialises
	if (glfwInit() == false)
	{
		return;
	}

	// creates the window
	m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (m_window == nullptr)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	// remaps all of OpenGl's function calls to the correct versions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	// prints the current version
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	if (!startup())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	// sets the background colour
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	aie::Gizmos::create(256, 256, 32768, 32768);

	// variables for timing
	double prevTime = glfwGetTime();
	double currTime = 0;
	double deltaTime = 0;
	unsigned int frames = 0;
	double fpsInterval = 0;

	while (!m_gameOver)
	{
		// GL_COLOR_BUFFER_BIT informs OpenGL tp wipe the back-buffer colours clean
		// GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// clears all previously drawn gizmos
		aie::Gizmos::clear();

		// update delta time
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;

		update(float(deltaTime));
		draw();

		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	shutdown();
	aie::Gizmos::destroy();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}