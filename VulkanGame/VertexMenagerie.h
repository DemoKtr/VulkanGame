#pragma once
#include "config.h"
#include "Memory.h"
#include <unordered_map>


struct  FinalizationChunk {
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;

};

class VertexMenagerie
{
	int offset;
	vk::Device logicalDevice;
	std::vector<float> lump;
	public:
		VertexMenagerie();
		~VertexMenagerie();
		void consume(meshTypes meshType, std::vector<float> data);
		void finalize(FinalizationChunk finalizationChink);
		Buffer vertexBuffer;
		std::unordered_map<meshTypes, int> offsets;
		std::unordered_map<meshTypes, int> sizes;

};

