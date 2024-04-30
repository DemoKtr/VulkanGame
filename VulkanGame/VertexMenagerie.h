#pragma once
#include "config.h"
#include "Memory.h"
#include <unordered_map>

class VertexMenagerie
{
	int offset;
	vk::Device logicalDevice;
	std::vector<float> lump;
	public:
		VertexMenagerie();
		~VertexMenagerie();
		void consume(meshTypes meshType, std::vector<float> data);
		void finalize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
		Buffer vertexBuffer;
		std::unordered_map<meshTypes, int> offsets;
		std::unordered_map<meshTypes, int> sizes;

};

