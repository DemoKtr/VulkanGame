#include "app.h"
#include <sstream>
void App::build_glfw_window(ivec2 screenSize, bool debugMode)
{
	glfwInit();

	//no default rendering client, we'll hook vulkan up
	//to the window later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//resizing breaks the swapchain, we'll disable it for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	//GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	if (window = glfwCreateWindow(screenSize.x, screenSize.y, "VulkanGame", nullptr, nullptr)) {
		if (debugMode) {
			std::cout << "Successfully made a glfw window , width: " << screenSize.x << ", height: " << screenSize.y << '\n';
		}
	}
	else {
		if (debugMode) {
			std::cout << "GLFW window creation failed\n";
		}
	}
}

void App::calculateFrameRate()
{
	currentTime = glfwGetTime();
	double delta = currentTime - lastTime;

	if (delta >= 1) {
		int framerate{ std::max(1, int(numFrames / delta)) };
		
		std::stringstream title{};
		title << "Running at " << framerate << " fps.  There are"<< verticesCounter << "vertices send to gpu";
		glfwSetWindowTitle(window, title.str().c_str());
		
		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(1000.0 / framerate);
	}

	++numFrames;
}

App::App(ivec2 screenSize, bool debugMode)
{
	build_glfw_window(screenSize, debugMode);

	scene = new Scene();
	
	graphicsEngine = new GraphicsEngine(screenSize, window, scene, debugMode);
	
}

App::~App()
{
	delete graphicsEngine;
	delete scene;
}

void App::run()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		graphicsEngine->render(scene,verticesCounter);
		calculateFrameRate();
	}
}
