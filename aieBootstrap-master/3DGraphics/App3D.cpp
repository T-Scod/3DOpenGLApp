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
	// adds the lights to the collection of lights
	m_lights.push_back(pointLight);
	m_lights.push_back(directionalLight);

	return true;
}
/*
	\fn void shutdown()
	\brief Deletes pointers.
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
	\brief Updates the camera each frame and makes an orbiting "sun" by moving the directional light around.
	\param deltaTime The time between each frame.
*/
void App3D::update(float deltaTime)
{
	// checks if the escape button was pressed
	if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// ends the game loop
		m_gameOver = true;
	}

	m_camera->Update(deltaTime);

	// directional light orbits the center of the area
	float time = getTime();
	// for directional lights the position property is proportional to the direction of the light
	m_lights[1].position = glm::vec4(glm::normalize(glm::vec3(glm::cos(time * 2.0f), glm::sin(time * 2.0f), 0.0f)), 0);
}
/*
	/fn void draw()
	/brief Draws all objects using the aie shader and gizmo class draw functions.
*/
void App3D::draw()
{
	// adds 3 coloured lines to the scene to represent the axis of the world space
	aie::Gizmos::addTransform(glm::mat4(1.0f));

	// predefined colours
	glm::vec4 white(1.0f);
	glm::vec4 black(0.0f, 0.0f, 0.0f, 1.0f);

	// adds a grid with black lines and a white line in the middle rows and columns
	for (int i = 0; i < 21; i++)
	{
		// x axis lines
		aie::Gizmos::addLine(glm::vec3(-10.0f + i, 0.0f, 10.0f),
			glm::vec3(-10.0f + i, 0.0f, -10.0f),
			(i == 10) ? white : black);
		// z axis lines
		aie::Gizmos::addLine(glm::vec3(10.0f, 0.0f, -10.0f + i),
			glm::vec3(-10.0f, 0.0f, -10.0f + i),
			(i == 10) ? white : black);
	}

	// gets the projection view matrix from the camera
	glm::mat4 pv = m_camera->GetProjectionView();
	// sets the phong shader as the current shader
	m_phongShader.bind();
	// sets the value of the bound shader uniforms
	m_phongShader.bindUniform("ProjectionViewModel", pv * m_spearTransform);
	m_phongShader.bindUniform("ModelMatrix", m_camera->GetModel());
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_phongShader.bindUniform("numLights", (int)m_lights.size());
	m_phongShader.bindUniform("cameraPosition", glm::vec3(m_camera->GetModel()[3]));

	// binds the property for each light in the collection
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		SetLightUniform(&m_phongShader, "position", i, m_lights[i].position);
		SetLightUniform(&m_phongShader, "Ia", i, m_lights[i].Ia);
		SetLightUniform(&m_phongShader, "Id", i, m_lights[i].Id);
		SetLightUniform(&m_phongShader, "Is", i, m_lights[i].Is);
		SetLightUniform(&m_phongShader, "attenuation", i, m_lights[i].attenuation);
	}
	// draws the model
	m_spearMesh.draw();

	// draws the point light in the same way that the spear is drawn
	m_simpleShader.bind();
	m_simpleShader.bindUniform("ProjectionViewModel", pv);
	m_mesh->Draw();

	// draws the directional light as a sphere using the gizmos class
	aie::Gizmos::addSphere(m_lights[1].position, 1.0f, 16.0f, 16.0f, { m_lights[1].Id, 1.0f });

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

/*
	\fn RunApp()
	\brief Initialises the application window and sets up a game loop.
*/
void App3D::RunApp()
{
	// initialises the GLFW library
	if (glfwInit() == false)
	{
		return;
	}

	// creates the application window
	m_window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
	// checks if the window was created
	if (m_window == nullptr)
	{
		glfwTerminate();
		return;
	}

	// makes the window a current on the calling thread
	glfwMakeContextCurrent(m_window);

	// remaps all of OpenGl's function calls to the correct versions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	// prints the openGL current version
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	// initialises the variables in the application and properly shuts down if an error occured
	if (!startup())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	// sets the background colour to grey
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	// enables depth calculations
	glEnable(GL_DEPTH_TEST);
	// enables particular sides of a tri to be culled
	glEnable(GL_CULL_FACE);

	// creates a gizmo instance
	aie::Gizmos::create(32768, 32768, 256, 256);

	// variables for timing
	double prevTime = glfwGetTime();
	double currTime = 0;
	double deltaTime = 0;
	unsigned int frames = 0;
	double fpsInterval = 0;

	// game loop that continues until the game is over
	while (!m_gameOver)
	{
		// GL_COLOR_BUFFER_BIT informs OpenGL tp wipe the back-buffer colours clean
		// GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// clears all previously drawn gizmos
		aie::Gizmos::clear();

		// update delta time by finding the time between frames
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;

		// passes delta time into the update
		update(float(deltaTime));
		// draws everything in the application
		draw();

		// processes the events in the application
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	// shuts down the program when the game is over
	shutdown();
	// destroys all gizmos and the window
	aie::Gizmos::destroy();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}