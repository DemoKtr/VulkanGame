#pragma once
#include "config.h"
#include <glm/fwd.hpp>
#include "Transform.h"
class SceneObject
{
protected:
	Transform transform;
public:
	 virtual void draw() = 0;
	 virtual void update() =0;
	 Transform& getTransform();
};



class Triangle : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update() override;

	Triangle();
};
class Star : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update() override;

	Star();
};
class Square : public SceneObject {
	// Inherited via StaticSceneObject
public:
	void draw() override;
	void update() override;

	Square();
};