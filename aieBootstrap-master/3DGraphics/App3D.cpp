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
	//m_camera->SetView(glm::inverse(m_camera->GetModel()));
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

	glm::vec3 position = { m_camera->GetModel()[3][0], m_camera->GetModel()[3][1], m_camera->GetModel()[3][2] };
	static float horizontalAngle = glm::pi<float>() * 0.25f;
	static float verticalAngle = 0.0f;
	static float initialFoV = 45.0f;
	float speed = 3.0f;
	float mouseSpeed = 0.005f;
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	glfwSetCursorPos(m_window, (double)width / 2.0, (double)height / 2.0);
	horizontalAngle += mouseSpeed * (float(width) / 2.0f - (float)xPos) * deltaTime;
	verticalAngle += mouseSpeed * (float(height) / 2.0f - (float)yPos) * deltaTime;
	float foo = cosf(verticalAngle);
	glm::vec3 direction(cosf(verticalAngle) * sinf(horizontalAngle),
						sinf(verticalAngle),
						cosf(verticalAngle) * cosf(horizontalAngle));
	glm::vec3 right(sinf(horizontalAngle - glm::pi<float>() / 2.0f),
					0.0f,
					cosf(horizontalAngle - glm::pi<float>() / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		position += direction * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		position -= direction * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		position += direction * speed * deltaTime;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		position -= direction * speed * deltaTime;
	}
	glfwSetScrollCallback(m_window, ScrollCallback);
	float FoV = initialFoV - 5.0f * m_scroll;
	//initialFoV = FoV;

	m_camera->Perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);
	m_camera->LookAt(position, position + direction, up);
	//m_camera->SetView(m_camera->GetModel());
}
void App3D::draw()
{
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

	glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 100.0f);
	glm::mat4 pvm = m_camera->GetPVM();
	pvm = projection * view * m_camera->GetModel();

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
		aie::Gizmos::draw(m_camera->GetPVM());

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