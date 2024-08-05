#pragma once
#include "config.h"
#include "AnimatedMesh.h"
#include "Bones.h"
#include <map>
#include "GLMConverter.h"
#include <assimp/scene.h>

class AnimatedModel
{
public:
	// model data 
	std::vector<vkMesh::AnimatedMesh>    meshes;
	std::string directory;

	// constructor, expects a filepath to a 3D model.
	AnimatedModel(const char* path);

	// draws the model, and thus all its meshes
	void Draw();

	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount();
	std::vector<vkMesh::AnimatedMesh> getMeshVertices();

private:

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(const char* path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	void SetVertexBoneDataToDefault(vkMesh::Vertex& vertex);


	vkMesh::AnimatedMesh processMesh(aiMesh* mesh, const aiScene* scene);

	

	void SetVertexBoneData(vkMesh::Vertex& vertex, int boneID, float weight);


	void ExtractBoneWeightForVertices(std::vector<vkMesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene);



};

