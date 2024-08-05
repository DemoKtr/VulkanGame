#include "AnimatedModel.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
AnimatedModel::AnimatedModel(const char* path)
{
	loadModel(path);
}
void AnimatedModel::Draw()
{
}

void AnimatedModel::loadModel(const char* path)
{
	// read file via ASSIMP
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath


	//directory = path.substr(0, pathStr.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void AnimatedModel::SetVertexBoneDataToDefault(vkMesh::Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

vkMesh::AnimatedMesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vkMesh::Vertex> vertices;
	std::vector<unsigned int> indices;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vkMesh::Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.Position = glm::vec4(Converter::GetGLMVec(mesh->mVertices[i]),1.0f);
		vertex.Normal = glm::vec4(Converter::GetGLMVec(mesh->mNormals[i]),1.0f);

		if (mesh->mTextureCoords[0])
		{
			glm::vec4 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vec.z = 0.0f;
			vec.w = 1.0f;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec4(0.0f, 0.0f,0.0f,0.0f);

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	ExtractBoneWeightForVertices(vertices, mesh, scene);

	return vkMesh::AnimatedMesh(vertices, indices);
}

int& AnimatedModel::GetBoneCount()
{
	return m_BoneCounter;
}

std::vector<vkMesh::AnimatedMesh> AnimatedModel::getMeshVertices()
{
	return meshes;
}



void AnimatedModel::SetVertexBoneData(vkMesh::Vertex& vertex, int boneID, float weight)
{

	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}

void AnimatedModel::ExtractBoneWeightForVertices(std::vector<vkMesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneInfoMap = m_BoneInfoMap;
	int& boneCount = m_BoneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = Converter::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}




