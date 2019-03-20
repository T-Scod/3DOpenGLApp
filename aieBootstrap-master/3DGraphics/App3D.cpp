#include "App3D.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

float m_scroll = 0.0f;

App3D::App3D()
{
}
App3D::~App3D()
{
}

bool App3D::startup()
{
	m_camera = new Camera();
	m_camera->LookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->Perspective(glm::pi<float>() * 0.25f, 1280.0f / 720.0f, 0.1f, 100.0f);

	return true;
}
void App3D::shutdown()
{
	delete m_camera;
	m_camera = nullptr;
}

void App3D::update(float deltaTime)
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
	glm::vec2 mouseDir = { ((double)width / 2.0) - xPos, ((double)height / 2.0) - yPos};

	m_camera->Rotate(mouseDir.x * mouseSpeed * deltaTime, m_camera->GetView() * glm::vec4({ 0.0f, 1.0f, 0.0f, 0.0f }));
	m_camera->Rotate(mouseDir.y * mouseSpeed * deltaTime, { 1.0f, 0.0f, 0.0f });

	static float initialFoV = 45.0f;
	glfwSetScrollCallback(m_window, ScrollCallback);
	float FoV = initialFoV - 5.0f * m_scroll;

	m_camera->Perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);
}
void App3D::draw()
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

	//aie::Gizmos::draw(projection * view);
	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void App3D::RunApp()
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

	//glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 100.0f);
	//glm::mat4 pvm = m_camera->GetProjectionView();
	//pvm = projection * view * m_camera->GetModel();

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

void App3D::ScrollCallback(GLFWwindow * window, double x, double y)
{
	m_scroll += (float)y;
}