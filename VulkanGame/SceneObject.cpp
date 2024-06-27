#include "SceneObject.h"

//SceneObject



void SceneObject::update(float deltaTime)
{
	
}

Transform& SceneObject::getTransform()
{
	return this->transform;
}

void Box::draw()
{
}

void Box::update(float deltaTime)
{
	angle += 0.1f * deltaTime;
	if(index==0)
	transform.setLocalPosition(glm::vec3(glm::sin(angle)*0.5f, 0.0f, glm::cos(angle)*.5f));
	else transform.setLocalPosition(glm::vec3(glm::cos(angle)*0.5f, 0.0f, glm::sin(angle)*0.5f));
	transform.rotate(glm::vec3(0.0f,1.0f,0.0f),0.1*deltaTime);
	transform.computeModelMatrix();
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

void Floor::update(float deltaTime)
{
	
}

Floor::Floor()
{
	objMaterial.setFilenames("res/models/plane.obj", "res/models/plane.mtl", "res/textures/groundAlbedo.jpg","res/textures/groundNormal.jpg", "res/textures/groundAMR.jpg", "res/textures/groundDepth.jpg");
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
	angle += 0.0001f;
	transform.setLocalPosition(glm::vec3(glm::cos(angle)*0.5f,1,glm::sin(angle)*0.5f));
	
	transform.computeModelMatrix();
}

Light::~Light()
{
}
