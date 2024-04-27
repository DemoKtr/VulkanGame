#include "SceneObject.h"

//SceneObject

void SceneObject::rotate(glm::vec4 rotation)
{
	this->transform.rotate(glm::vec3(rotation.y, rotation.z, rotation.w), rotation.x);
}

void SceneObject::translate(glm::vec3 translate)
{
	glm::vec3 localPos =this->transform.getLocalPosition();
	this->transform.setLocalPosition(localPos + translate);
}

void SceneObject::scale(glm::vec3 scale)
{
	this->transform.setLocalScale(scale);
}

void SceneObject::update()
{
	
}
// Static Scene Object
void StaticSceneObject::draw()
{

}
// Dynamic Scene Object
void DynamicSceneObject::draw()
{
}