#pragma once
#include "config.h"
#include <unordered_map>
#include "AnimatedMesh.h"


namespace vkMesh {

	struct Vert {
		// position
		glm::vec3 Position;
		// bitangent
		glm::vec3 Color;
		// texCoords
		glm::vec2 TexCoords;
		// normal
		glm::vec3 Normal;
		
		// tangent
		glm::vec3 Tangent;
		
	};


	class ObjMesh
	{
	public:
		std::vector<Vert> vertices;
		std::vector<uint32_t> indices;
		std::vector<glm::vec3> v, vn;
		std::vector<glm::vec2> vt;
		std::unordered_map<std::string, uint32_t> history;
		std::unordered_map<std::string, glm::vec3> colorLookup;
		std::unordered_map<std::string, glm::vec3> ambientLookup;
		std::unordered_map<std::string, glm::vec3> specularLookup;
		glm::vec3 brushColor;
		glm::vec3 ambientColor;
		glm::vec3 specularColor;
		glm::mat4 preTransform;

		ObjMesh(const char* objFilepath, const char* mtlFilepath, glm::mat4 preTransform);

		void read_vertex_data(const std::vector<std::string>& words);

		void read_texcoord_data(const std::vector<std::string>& words);

		void read_normal_data(const std::vector<std::string>& words);

		void read_face_data(const std::vector<std::string>& words);

		glm::vec3 readBtangent(const std::string& first_vertex_description, const std::string& seccond_vertex_description, const std::string& third_vertex_description);
		
		void read_corner(const std::string& vertex_description,glm::vec3 tangent);
	};



}


