#pragma once
#include "config.h"
class GraphicsEngine
{
	ivec2 screenSize{ 640, 480 };
	bool debugMode = { true };
	GLFWwindow* mainWindow{ nullptr };
		


	void build_glfw_window();
public:
	GraphicsEngine();
	~GraphicsEngine();
};

