#pragma once
#include <vector>
#include <deque>
#include "SceneObject.h"
class Scene
{
	std::vector<SceneObject*> staticSceneObjects;
	std::deque<SceneObject*> dynamicSceneObjects;

public:
	Scene();
	~Scene();
};

