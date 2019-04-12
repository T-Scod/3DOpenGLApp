#include "RenderingApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <glm/gtx/transform.hpp>

RenderingApp::RenderingApp()
{
}
RenderingApp::~RenderingApp()
{
}

bool RenderingApp::startup()
{
	m_camera = new Camera();
	m_camera->LookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->Perspective(glm::pi<float>() * 0.25f, 1280.0f / 720.0f, 0.1f, 100.0f);

	//m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/simpleColour.vert");
	//m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/simpleColour.frag");
	//if (m_simpleShader.link() == false)
	//{
	//	printf("Shader Error: %s\n", m_simpleShader.getLastError());
	//	return false;
	//}
	//m_textureShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/simpleTexture.vert");
	//m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/simpleTexture.frag");
	//if (m_textureShader.link() == false)
	//{
	//	printf("Texture Shader Error: %s\n", m_textureShader.getLastError());
	//	return false;
	//}
	//m_spearShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/simpleTexture.vert");
	//m_spearShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/simpleTexture.frag");
	//if (m_spearShader.link() == false)
	//{
	//	printf("Texture Shader Error: %s\n", m_spearShader.getLastError());
	//	return false;
	//}
	/*m_phongShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		printf("Phong Shader Error!\n", m_phongShader.getLastError());
		return false;
	}*/
	m_normalShader.loadShader(aie::eShaderStage::VERTEX, "../bin/shaders/normalMap.vert");
	m_normalShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/shaders/normalMap.frag");
	if (m_normalShader.link() == false)
	{
		printf("Normal Shader Error!\n", m_normalShader.getLastError());
		return false;
	}

	//m_transform = new glm::mat4({ 10, 0, 0, 0,
	//							  0, 10, 0, 0,
	//							  0, 0, 10, 0,
	//							  0, 0, 0, 1 });
	//m_mesh = new Mesh(1000, 1000);
	//m_mesh->AddBox(glm::vec3(-20.0f, 0.0f, 20.0f), glm::vec3(0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	//m_mesh->AddCylinder(glm::vec3(-20.0f, 0.0f, -20.0f), 0.5f, 0.5f, 10, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	//m_mesh->AddPyramid(glm::vec3(20.0f, 0.0f, -20.0f), 0.5f, 0.5f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);
	//m_mesh->AddSphere(glm::vec3(20.0f, 0, 20.0f), 0.5f, 16, 16, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), m_transform);

	//if (m_gridTexture.load("../bin/textures/numbered_grid.tga") == false)
	//{
	//	printf("Failed to load texture!\n");
	//	return false;
	//}
	//m_quadMesh = new Mesh(2, 4);
	//m_quadMesh->AddQuadTextured(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	//if (m_objMesh.load("../bin/stanford/buddha.obj") == false)
	//{
	//	printf("Bunny Mesh Error!\n");
	//	return false;
	//}
	//m_objTransform = { 0.5f, 0.0f, 0.0f, 0.0f,
	//					 0.0f, 0.5f, 0.0f, 0.0f,
	//					 0.0f, 0.0f, 0.5f, 0.0f,
	//					 -0.08f, 0.0f, -0.07f, 1.0f };

	if (m_spearMesh.load("../bin/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}
	m_spearTransform = { 2.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, 2.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 2.0f, 0.0f,
						 1.0f, 0.0f, 0.0f, 1.0f };

	//m_light.diffuse = { 1.0f, 1.0f, 0.0f };
	//m_light.specular = { 1.0f, 1.0f, 0.0f };
	//m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_light.position = glm::vec3(m_camera->GetModel()[3]);
	m_light.intensities = glm::vec3(1.0f);

	Light spotLight;
	spotLight.position = glm::vec4(-4.0f, 0.0f, 10.0f, 1.0f);
	spotLight.intensities = glm::vec3(2.0f, 2.0f, 2.0f);
	spotLight.attenuation = 0.1f;
	spotLight.ambientCoefficient = 0.0f;
	spotLight.coneAngle = 15.0f;
	spotLight.coneDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	Light directionalLight;
	directionalLight.position = glm::vec4(1.0f, 0.8f, 0.6f, 0.0f);
	directionalLight.intensities = glm::vec3(0.4f, 0.3f, 0.1f);
	directionalLight.ambientCoefficient = 0.06f;

	m_lights.push_back(spotLight);
	m_lights.push_back(directionalLight);

	return true;
}
void RenderingApp::shutdown()
{
	delete m_camera;
	m_camera = nullptr;
	//delete m_mesh;
	//m_mesh = nullptr;
	//delete m_transform;
	//m_transform = nullptr;
}

void RenderingApp::update(float deltaTime)
{
	if (glfwWindowShouldClose(m_window) == true || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_gameOver = true;
	}

	m_camera->Update(deltaTime);

	//float time = getTime();
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2.0f), glm::sin(time * 2.0f), 0.0f));

	if (glfwGetKey(m_window, '1'))
	{
		m_light.position = glm::vec3(m_camera->GetModel()[3]);
	}

	if (glfwGetKey(m_window, '2'))
	{
		m_light.intensities = glm::vec3(1.0f, 0.0f, 0.0f); // red
	}
	else if (glfwGetKey(m_window, '3'))
	{
		m_light.intensities = glm::vec3(0.0f, 1.0f, 0.0f); // green
	}
	else if (glfwGetKey(m_window, '4'))
	{
		m_light.intensities = glm::vec3(1.0f, 1.0f, 1.0f); // white
	}
}
void RenderingApp::draw()
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
	//m_simpleShader.bind();
	//m_simpleShader.bindUniform("ProjectionViewModel", pvm);
	//m_mesh->Draw();

	//m_spearShader.bind();
	//m_spearShader.bindUniform("ProjectionViewModel", pvm);
	m_normalShader.bind();
	m_normalShader.bindUniform("ProjectionViewModel", pvm * m_spearTransform);
	m_normalShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_normalShader.bindUniform("ModelMatrix", m_camera->GetModel());
	m_normalShader.bindUniform("cameraPosition", glm::vec3(m_camera->GetModel()[3]));
	m_normalShader.bindUniform("numLights", (int)m_lights.size());

	for (size_t i = 0; i < m_lights.size(); i++)
	{
		SetLightUniform(m_normalShader, "position", i, m_lights[i].position);
		SetLightUniform(m_normalShader, "intensities", i, m_lights[i].intensities);
		SetLightUniform(m_normalShader, "attenuation", i, m_lights[i].attenuation);
		SetLightUniform(m_normalShader, "ambientCoefficient", i, m_lights[i].ambientCoefficient);
		SetLightUniform(m_normalShader, "coneAngle", i, m_lights[i].coneAngle);
		SetLightUniform(m_normalShader, "coneDirection", i, m_lights[i].coneDirection);
	}

	//m_phongShader.bind();
	//m_phongShader.bindUniform("light.position", m_light.position);
	//m_phongShader.bindUniform("light.intensities", m_light.intensities);
	m_spearMesh.draw();

	//m_textureShader.bind();
	//m_textureShader.bindUniform("ProjectionViewModel", pvm);
	//m_textureShader.bindUniform("diffuseTexture", 0);
	//m_gridTexture.bind(0);
	//m_quadMesh->Draw();

	//m_simpleShader.bind();
	//pvm = m_camera->GetProjectionView() * m_objTransform;
	//m_simpleShader.bindUniform("ProjectionViewModel", pvm);
	//m_objMesh.draw();

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void RenderingApp::RunApp()
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