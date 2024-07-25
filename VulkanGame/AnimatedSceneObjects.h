#pragma once
#include "SceneObject.h"
#include "Animator.h"
#include "AnimatedModel.h"
#include "Animator.h"
#include <unordered_map>


struct animatedobjectMaterial {
	animatedModelTypes meshType;
	char* model{};
	char* diffuse{};
	char* normalMap{};
	char* armMap{};
	char* depthMap{};
	void setFilenames(const char* newModel, const char* newDiffuse,const char* newNormalMap, const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		model = _strdup(newModel);
		diffuse = _strdup(newDiffuse);
		normalMap = _strdup(newNormalMap);
		armMap = _strdup(newArmMap);
		depthMap = _strdup(newDepthMap);

	}

	void clear() {
		delete[] model;
		delete[] diffuse;
		delete[] normalMap;
		delete[] armMap;
		delete[] depthMap;
	}

};

class AnimatedSceneObjects : public SceneObject
{

	

	public:
		animatedobjectMaterial AobjMaterial;
		vkAnimation::Animator* animator;
		std::unordered_map<std::string, vkAnimation::Animation> animations;
		void draw() override;
		void update(float deltaTime) override;
		void create_vertex_buffer();
		float angle = { 0 };
		int index = 0;
		AnimatedSceneObjects();
		~AnimatedSceneObjects();

};


