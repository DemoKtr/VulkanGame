#pragma once
#include "config.h"


namespace vkMesh {

#define MAX_BONE_INFLUENCE 4

    struct Vertex {
        // position
        glm::vec4 Position;
        // texCoords
        glm::vec4 TexCoords;
        // normal
        glm::vec4 Normal;
        // tangent
        glm::vec4 Tangent;
        // bitangent
        glm::vec4 Bitangent;
        //bone indexes which will influence this vertex
        glm::ivec4 m_BoneIDs;
        //weights from each bone
        glm::vec4 m_Weights;
    };

   
class AnimatedMesh
{
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<float> vertexLump;
    std::vector<int> bonesID;
    

    unsigned int VAO;

    // constructor
    AnimatedMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    {
        this->vertices = vertices;
        this->indices = indices;



        setupMesh();
    }

    // render the mesh
    void Draw()
    {
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        for (Vertex vert : vertices) {
            vertexLump.push_back(vert.Position.x);
            vertexLump.push_back(vert.Position.y);
            vertexLump.push_back(vert.Position.z);
            vertexLump.push_back(vert.Normal.x);
            vertexLump.push_back(vert.Normal.y);
            vertexLump.push_back(vert.Normal.z);
            vertexLump.push_back(vert.TexCoords.x);
            vertexLump.push_back(vert.TexCoords.y);
            vertexLump.push_back(vert.Tangent.x);
            vertexLump.push_back(vert.Tangent.y);
            vertexLump.push_back(vert.Tangent.z);
            vertexLump.push_back(vert.Bitangent.x);
            vertexLump.push_back(vert.Bitangent.y);
            vertexLump.push_back(vert.Bitangent.z);
            bonesID.push_back(vert.m_BoneIDs[0]);
            bonesID.push_back(vert.m_BoneIDs[1]);
            bonesID.push_back(vert.m_BoneIDs[2]);
            bonesID.push_back(vert.m_BoneIDs[3]);
            vertexLump.push_back(vert.m_Weights[0]);
            vertexLump.push_back(vert.m_Weights[1]);
            vertexLump.push_back(vert.m_Weights[2]);
            vertexLump.push_back(vert.m_Weights[3]);

        }

    }
};

}