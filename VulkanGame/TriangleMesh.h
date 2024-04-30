#pragma once
#include "config.h"
#include "Memory.h"
class TriangleMesh
{

public:
	TriangleMesh(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	~TriangleMesh();
	Buffer vertexBuffer;
private:
	vk::Device logicalDevice;
};


class SquareMesh {
public:
	SquareMesh(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	~SquareMesh();
	Buffer vertexBuffer;
private:
	vk::Device logicalDevice;
};

class StarMesh {
public:
	StarMesh(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	~StarMesh();
	Buffer vertexBuffer;
private:
	vk::Device logicalDevice;
};