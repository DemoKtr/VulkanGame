#pragma once
#include "GraphicsEngine.h"
#include "config.h"
class App
{
	GraphicsEngine* graphicsEngine;
	GLFWwindow* window;
	Scene* scene;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	double lastTime, currentTime;
	int numFrames;
	float frameTime;
	void build_glfw_window(ivec2 screenSize, bool debugMode);
	void calculateFrameRate();
	int verticesCounter = {};
	public:
		App(ivec2 screenSize, bool debugMode);
		~App();
		void run();
};

