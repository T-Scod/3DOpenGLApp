#pragma once
#include "Application.h"
#include "Camera.h"

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
};