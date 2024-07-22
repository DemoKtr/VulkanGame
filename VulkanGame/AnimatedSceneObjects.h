#pragma once
#include "SceneObject.h"
#include "Animator.h"
#include "AnimatedModel.h"


struct animatedobjectMaterial {
	animatedModelTypes meshType;
	char* model{};
	void setFilenames(const char* newModel) {
		// Zwolnienie poprzednio alokowanej pamiêci (jeœli istnieje)
		model = _strdup(newModel);

	}

	void clear() {
		delete[] model;
	}

};

class AnimatedSceneObjects : public SceneObject
{

	

	public:
		animatedobjectMaterial AobjMaterial;
		void draw() override;
		void update(float deltaTime) override;
		void create_vertex_buffer();
		float angle = { 0 };
		int index = 0;
		AnimatedSceneObjects();
		~AnimatedSceneObjects();

};


