#pragma once
#include "config.h"
#include "Memory.h"
#include <unordered_map>
#include "AnimatedMesh.h"
#include "ObjMesh.h"


struct  FinalizationChunk {
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;

};

class VertexMenagerie
{
	int indexOffset;
	bool isAnimated{ false };
	vk::Device logicalDevice;
	std::vector<vkMesh::Vert> staticVertex;
	std::vector<float> vertexLump;
	std::vector<vkMesh::Vertex> animatedVertex;
	std::vector<uint32_t> indexLump;


	public:
		VertexMenagerie();
		~VertexMenagerie();
		void consume(meshTypes meshType, std::vector<vkMesh::Vert> data, std::vector<uint32_t> indices);
		void consume(animatedModelTypes meshType, std::vector<vkMesh::Vertex> data, std::vector<uint32_t> indices);

		void finalize(FinalizationChunk finalizationChink);
		Buffer vertexBuffer, indexBuffer;
		std::unordered_map<meshTypes, int> firstIndices;
		std::unordered_map<animatedModelTypes, int> AfirstIndices;
		std::unordered_map<meshTypes, int> indexCounts;
		std::unordered_map<animatedModelTypes, int> AindexCounts;

};

