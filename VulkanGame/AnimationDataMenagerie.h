#pragma once
#include "config.h"
#include <unordered_map>
#include "AnimatedModel.h"
#include "VertexMenagerie.h"
class AnimationDataMenagerie
{

	int indexOffset;
	vk::Device logicalDevice;
	std::vector<vkMesh::Vertex> vertexLump;
	std::vector<uint32_t> indexLump;
public:
	AnimationDataMenagerie();
	~AnimationDataMenagerie();
	void consume(meshTypes meshtype,std::vector<vkMesh::Vertex> data, std::vector<uint32_t> indicies);
	void finalize(FinalizationChunk finalizationChunk);
	Buffer vertexBuffer, indexBuffer;
	std::unordered_map<meshTypes, int> firstIndices;
	std::unordered_map<meshTypes, int> indexCounts;


};

