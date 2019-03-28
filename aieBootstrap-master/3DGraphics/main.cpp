#include "RenderingApp.h"
#include <GLFW/glfw3.h>

int main()
{
	RenderingApp* app = new RenderingApp();
	app->RunApp();

	delete app;
	return 0;
}