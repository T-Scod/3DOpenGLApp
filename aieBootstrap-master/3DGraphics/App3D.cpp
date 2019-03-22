#include "App3D.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

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

	m_shader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/simple.frag");

	if (m_shader.link() == false)
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	m_transform = new glm::mat4({ 5, 0, 0, 0,
									  0, 5, 0, 0,
									  0, 0, 5, 0,
									  0, 0, 0, 1 });

	m_mesh = new Mesh(1000);
	m_mesh->AddBox(glm::vec3(-10.0f, 0.0f, 10.0f), glm::vec3(0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	m_mesh->AddBox(glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	m_mesh->AddCylinder(glm::vec3(-10.0f, 0.0f, -10.0f), 0.5f, 0.5f, 10, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	m_mesh->AddPyramid(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	m_mesh->AddSphere(glm::vec3(10.0f, 0, 10.0f), 0.5f, 16, 16, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);

	return true;
}
void App3D::shutdown()
{
	delete m_camera;
	m_camera = nullptr;
	delete m_mesh;
	m_mesh = nullptr;
	delete m_transform;
	m_transform = nullptr;
}

void App3D::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_gameOver = true;
	}

	m_camera->Update(deltaTime);
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

	// bind shader
	m_shader.bind();
	// bind transform
	glm::mat4 pv = m_camera->GetProjectionView();
	m_shader.bindUniform("ProjectionViewModel", pv);
	// draw quad
	m_mesh->Draw();

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
	glEnable(GL_CULL_FACE);

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