#pragma once
#include <vector>
#include <deque>
#include "SceneObject.h"
#include <unordered_map>
class Scene
{


public:
	Scene();
	~Scene();
	std::unordered_map <meshTypes, std::vector<SceneObject*>> positions;

};

