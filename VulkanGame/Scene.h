#pragma once
#include <vector>
#include <deque>
#include "SceneObject.h"
#include <unordered_map>
#include "ParticleEmiter.h"
#include "AnimatedSceneObjects.h"



class Scene
{


public:
	Scene();
	~Scene();
	//std::unordered_map <meshTypes, std::vector<SceneObject*>> positions;
	std::vector<SceneObject*> sceneObjects;
	std::vector<AnimatedSceneObjects*> animatedSceneObjects;
	std::vector<Light*> lights;
	std::vector<ParticleEmiter*> particleEmiters;
	void updateScene(float deltaTime);
};

