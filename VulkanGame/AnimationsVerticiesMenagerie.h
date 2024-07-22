#pragma once
#include "config.h"
#include <unordered_map>
#include "AnimatedMesh.h"
struct  FinalizationChunk {
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;

};

class AnimationsVerticiesMenagerie
{
	int indexOffset;
	vk::Device logicalDevice;
	std::vector<vkMesh::Vertex> vertexLump;
	std::vector<uint32_t> indexLump;
public:
	AnimationsVerticiesMenagerie();
	~AnimationsVerticiesMenagerie();
	void consume(meshTypes meshType, std::vector<float> data, std::vector<uint32_t> indicies);
	void finalize(FinalizationChunk finalizationChink);
	Buffer vertexBuffer, indexBuffer;
	std::unordered_map<meshTypes, int> firstIndices;
	std::unordered_map<meshTypes, int> indexCounts;
};

