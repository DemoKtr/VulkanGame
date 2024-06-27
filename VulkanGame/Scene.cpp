#include "Scene.h"

Scene::Scene()
{
	
	
	//positions.insert({ meshTypes::KITTY, {} });
	//positions.insert({ meshTypes::DOG, {} });
	//.insert({ meshTypes::KITTY, {} });
	
	
	Box* box = new Box();
	Box* box1 = new Box();
	box1->index = 1;
	Box* box2 = new Box();
	



	box->getTransform().setLocalPosition(glm::vec3(0.0f, -0.0f, 0.0f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box->getTransform().setLocalScale(glm::vec3(0.1f,0.1f,0.1f));
	box->getTransform().computeModelMatrix();

	box1->getTransform().setLocalPosition(glm::vec3(0.2f, -0.1f, 0.3f));
	//box->getTransform().rotate(glm::vec3(0.0f, 0.0f, 1.0f),(22.0f/28.0f));
	box1->getTransform().setLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
	box1->getTransform().computeModelMatrix();
	
	for (int x = -5; x < 5; ++x) {
		for (int z = -5; z < 5; ++z) {
			Floor* floor = new Floor();
			floor->getTransform().setLocalPosition(glm::vec3(static_cast<float>(x*2.01f), 0.0f, static_cast<float>(z*2.01f)));
			floor->getTransform().computeModelMatrix();
			sceneObjects.push_back(floor);
		}
	}
	

	//positions[meshTypes::KITTY].push_back(box);
	//positions[meshTypes::DOG].push_back(box1);
	//Floor* floor = new Floor();
	//floor->getTransform().setLocalPosition(glm::vec3(0.0f,0.0f,0.0f));
	//floor->getTransform().setLocalScale(glm::vec3(20.0f,1.0f,20.0f));
	//floor->getTransform().computeModelMatrix();
	
	sceneObjects.push_back(box);
	sceneObjects.push_back(box1);
	//sceneObjects.push_back(floor);
	



	Light* pointLight1 = new Light();
	Light* pointLight2 = new Light();
	pointLight1->diffuse = glm::vec4(28.0f, 28.0f, 28.0f,1.0f);
	pointLight2->diffuse = glm::vec4(28.0f, 28.0f, 28.0f,1.0f);
	pointLight1->transform.setLocalPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight1->transform.computeModelMatrix();
	pointLight2->transform.setLocalPosition(glm::vec3(5.0f, 1.0f,-3.0f));
	pointLight2->transform.computeModelMatrix();
	
	lights.push_back(pointLight1);
	lights.push_back(pointLight2);


	ParticleEmiter *p1 = new ParticleEmiter(glm::vec3(0.7f,0.0f,0.6f));

	particleEmiters.push_back(p1);


}

Scene::~Scene()
{
	for (SceneObject* obj : sceneObjects) ;
	for (Light* light : lights) ;

	for (ParticleEmiter* obj : particleEmiters) {
		delete obj;
	}
	
}

void Scene::updateScene(float deltaTime)
{
	for (SceneObject* obj : sceneObjects) {
		obj->update(deltaTime);
	}

	for (Light* light : lights) {

	}
}
