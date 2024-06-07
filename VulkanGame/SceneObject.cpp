#include "SceneObject.h"

//SceneObject



void SceneObject::update()
{
	
}

Transform& SceneObject::getTransform()
{
	return this->transform;
}

void Box::draw()
{
}

void Box::update()
{
}

Box::Box()
{
	objMaterial.setFilenames("res/models/box.obj", "res/models/box.mtl", "tex/floordif.png", "tex/floornormal.png", "tex/floorarm.png", "tex/floordepth.png");
	objMaterial.meshType = meshTypes::KITTY;
}

Box::~Box()
{
	objMaterial.clear();
}

void Floor::draw()
{
}

void Floor::update()
{
}

Floor::Floor()
{
	objMaterial.setFilenames("res/models/plane.obj", "res/models/plane.mtl", "tex/floordif.png", "tex/floornormal.png", "tex/floorarm.png", "tex/floordepth.png");
	objMaterial.meshType = meshTypes::DOG;
}

Floor::~Floor()
{
	objMaterial.clear();
}

Light::Light()
{
	diffuse = glm::vec4(255.0f,255.0f,255.0f,1.0f);
	transform.setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Light::move()
{
	angle += 0.001f;
	transform.setLocalPosition(glm::vec3(glm::cos(angle)*0.5f,1,glm::sin(angle)));
//	std::cout << glm::cos(angle) << std::endl;
	transform.computeModelMatrix();
}

Light::~Light()
{
}
