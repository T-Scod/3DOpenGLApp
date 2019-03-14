#include "App3D.h"
#include <GLFW/glfw3.h>

int main()
{
	App3D* app = new App3D();
	app->RunApp();

	delete app;
	return 0;
}