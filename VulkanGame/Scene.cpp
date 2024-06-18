#include "Scene.h"

Scene::Scene()
{
	
	
	//positions.insert({ meshTypes::KITTY, {} });
	//positions.insert({ meshTypes::DOG, {} });
	//.insert({ meshTypes::KITTY, {} });
	
	
	Box* box = new Box();
	Box* box1 = new Box();
	Box* box2 = new Box();
	Floor* floor = new Floor();



	box->getTransform().setLocalPosition(glm::vec3(0.0f, -0.0f, 0.0f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box->getTransform().setLocalScale(glm::vec3(0.1f,0.1f,0.1f));
	box->getTransform().computeModelMatrix();

	box1->getTransform().setLocalPosition(glm::vec3(0.2f, -0.1f, 0.3f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box1->getTransform().setLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
	box1->getTransform().computeModelMatrix();

	box2->getTransform().setLocalPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	box2->getTransform().setLocalScale(glm::vec3(1.0f, 0.1f, 1.0f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box2->getTransform().computeModelMatrix();



	//positions[meshTypes::KITTY].push_back(box);
	//positions[meshTypes::DOG].push_back(box1);

	sceneObjects.push_back(box);
	sceneObjects.push_back(box1);
	sceneObjects.push_back(box2);



	Light* pointLight1 = new Light();
	Light* pointLight2 = new Light();
	pointLight1->diffuse = glm::vec4(255.0f, 255.0f, 255.0f,1.0f);
	pointLight2->diffuse = glm::vec4(255.0f, 255.0f, 255.0f,1.0f);
	pointLight1->transform.setLocalPosition(glm::vec3(1.0f, 3.0f, 0.0f));
	pointLight1->transform.computeModelMatrix();
	pointLight2->transform.setLocalPosition(glm::vec3(0.0f, -100.0f,0.0f));
	pointLight2->transform.computeModelMatrix();
	
	lights.push_back(pointLight1);
	lights.push_back(pointLight2);


	ParticleEmiter *p1;

	particleEmiters.push_back(p1);


}

Scene::~Scene()
{
	for (SceneObject* obj : sceneObjects) ;
	for (Light* light : lights) ;
	
}
