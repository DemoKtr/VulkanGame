#pragma once
#include "config.h"
#include <glm/fwd.hpp>
#include "Transform.h"
class SceneObject
{
	Transform transform;
public:
	 virtual void draw() = 0;
	 void rotate(glm::vec4 rotation);
	 void translate(glm::vec3 translate);
	 void scale(glm::vec3 scale);
	 virtual void update();
	
};

class StaticSceneObject : SceneObject{
	// Inherited via SceneObject
	void draw() override;
};

class DynamicSceneObject : SceneObject {
	// Inherited via SceneObject
	void draw() override;
};