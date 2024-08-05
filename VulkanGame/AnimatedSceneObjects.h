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

	std::vector<std::string> animationFilenames;

	void setFilenames(const char* newModel, const char* newDiffuse,const char* newNormalMap, const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		model = _strdup(newModel);
		diffuse = _strdup(newDiffuse);
		normalMap = _strdup(newNormalMap);
		armMap = _strdup(newArmMap);
		depthMap = _strdup(newDepthMap);

	}

	void setAnimationFilenames(std::vector<std::string> filenames) {
		animationFilenames = filenames;
	}

	void clear() {
		delete[] model;
		delete[] diffuse;
		delete[] normalMap;
		delete[] armMap;
		delete[] depthMap;
	}

};

class AnimatedObject {

public:

	vkAnimation::Animator* animator;
	std::unordered_map<std::string, vkAnimation::Animation*> animations;
	animatedobjectMaterial AobjMaterial;
	void UpdateAnimation(float deltaTime) {
		animator->UpdateAnimation(deltaTime);
	 }
	void PlayAnimation(std::string animationName) {
		if ((animations.find(animationName) != animations.end())) {
			animator->PlayAnimation(animations[animationName]);
		}
		else {
			std::cout << "Animation" + animationName + " does not exist" << std::endl;
		}
	}
	void loadAnimations(std::vector<std::string> filenames,AnimatedModel model) {
		for (std::string filename : filenames) {
			animations[filename] = new vkAnimation::Animation("res/animations/"+filename, &model);
		}

	}

};


class AnimatedSceneObjects : public SceneObject, public AnimatedObject
{
	public:
		std::vector<std::string> animationFilenames;
		void draw() override;
		void update(float deltaTime) override;
		void create_vertex_buffer();
		void load_animations(AnimatedModel model);
		float angle = { 0 };
		int index = 0;

		AnimatedSceneObjects();
		~AnimatedSceneObjects();

};


