#pragma once
#include <vector>
#include <deque>
#include "SceneObject.h"
#include <unordered_map>
#include "ParticleEmiter.h"



class Scene
{


public:
	Scene();
	~Scene();
	//std::unordered_map <meshTypes, std::vector<SceneObject*>> positions;
	std::vector<SceneObject*> sceneObjects;
	std::vector<Light*> lights;
	std::vector<ParticleEmiter*> particleEmiters;
};

