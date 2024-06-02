#pragma once
#include "config.h"
#include <unordered_map>


namespace vkMesh {

	struct TangentBtangent {
		glm::vec3 tangent;
		glm::vec3 bTangent;
	};



	class ObjMesh
	{
	public:
		std::vector<float> vertices;
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

		TangentBtangent readBtangent(const std::string& first_vertex_description, const std::string& seccond_vertex_description, const std::string& third_vertex_description);
		
		void read_corner(const std::string& vertex_description);
	};



}


