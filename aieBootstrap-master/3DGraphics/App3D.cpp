/*
	\file App3D.cpp
	\brief A file of definitions for an application class.
*/
#include "App3D.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

/*
	\fn App3D()
	\brief Default constructor.
	\fn ~App3D()
	\brief Default destructor.
*/
App3D::App3D()
{
}
App3D::~App3D()
{
}

/*
	\fn bool startup()
	\brief Loads all the shaders and models as well as initialising the light sources.
	\brief This function is called only once before the start of the game loop.
	\brief Additionally, the function creates 2 light sources and adds them to the collection of lights.
	\return Returns false if something was not loaded properly.
*/
bool App3D::startup()
{
	// creates a new camera at location (10, 10, 10) that is looking towards the origin
	m_camera = new Camera();
	m_camera->LookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->Perspective(glm::pi<float>() * 0.25f, 1280.0f / 720.0f, 0.1f, 100.0f);

	// loads the phong shader from the bin folder and attempts to link it
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		// prints an error if the shader failed to load
		printf("Phong Shader Error!\n", m_phongShader.getLastError());
		return false;
	}
	// loads the simple shader from the bin folder and attempts to link it
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/simpleColour.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/simpleColour.frag");
	if (m_simpleShader.link() == false)
	{
		// prints an error if the shader failed to load
		printf("Colour Shader Error!\n", m_simpleShader.getLastError());
		return false;
	}

	// attempts to load the spear mesh with textures
	if (m_spearMesh.load("../bin/soulspear/soulspear.obj", true, true) == false)
	{
		// prints an error if it failed to load
		printf("Soulspear Mesh Error!\n");
		return false;
	}
	// initialises the transform of the spear to have a scale of 200%
	m_spearTransform = { 2.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, 2.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 2.0f, 0.0f,
						 1.0f, 0.0f, 0.0f, 1.0f };

	// cyan pyramid to represent the point light
	m_mesh = new Mesh(1000, 1000);
	m_mesh->AddPyramid(glm::vec3(-4.0f, 0.0f, 10.0f), 1.0f, 1.0f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	// cyan point light with little loss in light over distance
	Light pointLight;
	pointLight.position = glm::vec4(-4.0f, 0.0f, 10.0f, 1.0f);
	pointLight.Ia = glm::vec3(0.25f);
	pointLight.Id = glm::vec3(0.0f, 1.0f, 1.0f);
	pointLight.Is = glm::vec3(0.0f, 1.0f, 1.0f);
	pointLight.attenuation = 0.05f;
	// yellow directional light
	Light directionalLight;
	directionalLight.position = glm::vec4(-4.0f, 0.0f, 10.0f, 0.0f);
	directionalLight.Ia = glm::vec3(0.25f);
	directionalLight.Id = glm::vec3(1.0f, 1.0f, 0.0f);
	directionalLight.Is = glm::vec3(1.0f, 1.0f, 0.0f);
	directionalLight.attenuation = 1.0f;

	m_lights.push_back(pointLight);
	m_lights.push_back(directionalLight);

	return true;
}
/*
	\fn void shutdown()
	\brief Deallocates all pointers.
*/
void App3D::shutdown()
{
	delete m_camera;
	m_camera = nullptr;
	delete m_mesh;
	m_mesh = nullptr;
}

/*
	\fn void update(float deltaTime)
	\brief Updates the camera each frame.
	\param deltaTime The time between each frame.
*/
void App3D::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_gameOver = true;
	}

	m_camera->Update(deltaTime);

	// directional light orbits the center of the area
	float time = getTime();
	m_lights[1].position = glm::vec4(glm::normalize(glm::vec3(glm::cos(time * 2.0f), glm::sin(time * 2.0f), 0.0f)), 0);
}
/*
	/fn void draw()
	/brief Draws all objects.
*/
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

	glm::mat4 pvm = m_camera->GetProjectionView();
	m_phongShader.bind();
	// binds all the uniforms
	m_phongShader.bindUniform("ProjectionViewModel", pvm * m_spearTransform);
	m_phongShader.bindUniform("ModelMatrix", m_camera->GetModel());
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_phongShader.bindUniform("numLights", (int)m_lights.size());
	m_phongShader.bindUniform("cameraPosition", glm::vec3(m_camera->GetModel()[3]));

	// binds the property for each light
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		SetLightUniform(&m_phongShader, "position", i, m_lights[i].position);
		SetLightUniform(&m_phongShader, "Ia", i, m_lights[i].Ia);
		SetLightUniform(&m_phongShader, "Id", i, m_lights[i].Id);
		SetLightUniform(&m_phongShader, "Is", i, m_lights[i].Is);
		SetLightUniform(&m_phongShader, "attenuation", i, m_lights[i].attenuation);
	}
	m_spearMesh.draw();

	// draws the point light
	m_simpleShader.bind();
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);
	m_mesh->Draw();

	// draws the directional light as a sphere
	aie::Gizmos::addSphere(m_lights[1].position, 1.0f, 16.0f, 16.0f, { m_lights[1].Id, 1.0f });

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

/*
	\fn RunApp()
	\brief Sets up the window and starts the game loop.
*/
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

	aie::Gizmos::create(32768, 32768, 256, 256);

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