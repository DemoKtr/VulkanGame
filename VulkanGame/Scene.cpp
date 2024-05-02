#include "Scene.h"

Scene::Scene()
{
	
	positions.insert({ meshTypes::KITTY, {} });
	positions.insert({ meshTypes::DOG, {} });
	positions.insert({ meshTypes::KITTY, {} });
	
	Box* box = new Box();
	Box* box1 = new Box();


	box->getTransform().setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box->getTransform().setLocalScale(glm::vec3(0.1f,0.1f,0.1f));
	box->getTransform().computeModelMatrix();

	box1->getTransform().setLocalPosition(glm::vec3(0.2f, 0.0f, 0.9f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box1->getTransform().setLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
	box1->getTransform().computeModelMatrix();



	positions[meshTypes::KITTY].push_back(box);
	positions[meshTypes::DOG].push_back(box1);





}

Scene::~Scene()
{

	
}
