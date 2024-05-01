#pragma once
#include <vector>
#include <deque>
#include "SceneObject.h"
class Scene
{


public:
	Scene();
	~Scene();
	std::vector<Triangle*> triangles;
	std::vector<Star*> stars;
	std::vector<Square*> squares;

};

