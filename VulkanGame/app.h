#pragma once
#include "GraphicsEngine.h"
#include "config.h"
class App
{
	GraphicsEngine* graphicsEngine;
	GLFWwindow* window;

	double lastTime, currentTime;
	int numFrames;
	float frameTime;
	void build_glfw_window(ivec2 screenSize, bool debugMode);
	void calculateFrameRate();

	public:
		App(ivec2 screenSize, bool debugMode);
		~App();
		void run();
};

