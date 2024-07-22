#pragma once
#include "SceneObject.h"
#include "Animator.h"
#include "AnimatedModel.h"

class AnimatedSceneObjects : public SceneObject
{

	AnimatedModel* animatedModel;

	public:
		void draw() override;
		void update(float deltaTime) override;
		void create_vertex_buffer();
		float angle = { 0 };
		int index = 0;
		AnimatedSceneObjects(char* const& path);
		~AnimatedSceneObjects();

};


