#pragma once
#ifndef APP_H
#define APP_H



#include "GraphicsEngine.h"
#include "config.h"
#include "Camera.h"
class App
{
	GraphicsEngine* graphicsEngine;
	GLFWwindow* window;
	Scene* scene;
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
		Camera::Camera camera;
		float deltaTime = 0.0f;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);



#endif // !APP_H