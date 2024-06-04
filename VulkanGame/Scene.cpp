#include "Scene.h"

Scene::Scene()
{
	
	
	//positions.insert({ meshTypes::KITTY, {} });
	//positions.insert({ meshTypes::DOG, {} });
	//.insert({ meshTypes::KITTY, {} });
	
	Box* box = new Box();
	Box* box1 = new Box();
	Floor* floor = new Floor();



	box->getTransform().setLocalPosition(glm::vec3(0.0f, -0.1f, 0.0f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box->getTransform().setLocalScale(glm::vec3(0.1f,0.1f,0.1f));
	box->getTransform().computeModelMatrix();

	box1->getTransform().setLocalPosition(glm::vec3(0.2f, -0.1f, 0.3f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box1->getTransform().setLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
	box1->getTransform().computeModelMatrix();

	floor->getTransform().setLocalPosition(glm::vec3(0.2f, -0.2f, 0.3f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	floor->getTransform().computeModelMatrix();



	//positions[meshTypes::KITTY].push_back(box);
	//positions[meshTypes::DOG].push_back(box1);

	sceneObjects.push_back(box);
	sceneObjects.push_back(box1);
	sceneObjects.push_back(floor);



	Light* pointLight1 = new Light();
	Light* pointLight2 = new Light();

	pointLight1->transform.setLocalPosition(glm::vec3(0.0f, -200.0f, 0.0f));
	pointLight1->transform.computeModelMatrix();
	pointLight2->transform.setLocalPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	pointLight2->transform.computeModelMatrix();

	lights.push_back(pointLight1);
	lights.push_back(pointLight2);


}

Scene::~Scene()
{
	for (SceneObject* obj : sceneObjects) delete obj;
	for (Light* light : lights) delete light;
	
}
