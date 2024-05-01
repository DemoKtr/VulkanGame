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
	int indexOffset;
	vk::Device logicalDevice;
	std::vector<float> vertexLump;
	std::vector<uint32_t> indexLump;
	public:
		VertexMenagerie();
		~VertexMenagerie();
		void consume(meshTypes meshType, std::vector<float> data, std::vector<uint32_t> indicies);
		void finalize(FinalizationChunk finalizationChink);
		Buffer vertexBuffer, indexBuffer;
		std::unordered_map<meshTypes, int> firstIndices;
		std::unordered_map<meshTypes, int> indexCounts;

};

