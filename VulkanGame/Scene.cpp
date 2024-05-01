#include "Scene.h"

Scene::Scene()
{
	
	Triangle* triangle = new Triangle(); // Tworzenie obiektu tree1
    Star* star = new Star(); // Tworzenie obiektu tree2
	Square* square = new Square();;
	triangles.push_back(triangle);
	stars.push_back(star);
	squares.push_back(square);

	triangles[0]->getTransform().setLocalPosition(glm::vec3(-0.6f, -0.2f, 0));
	stars[0]->getTransform().setLocalPosition(glm::vec3(-0.6f,0.4f, 0));
	squares[0]->getTransform().setLocalPosition(glm::vec3(-0.6f, .6f, 0));

	triangles[0]->getTransform().computeModelMatrix();
	stars[0]->getTransform().computeModelMatrix();
	squares[0]->getTransform().computeModelMatrix();
	



}

Scene::~Scene()
{
	for (Triangle *sb : triangles) {
		delete sb;
	}
	for (Star* sb : stars) {
		delete sb;
	}
	for (Square* sb : squares) {
		delete sb;
	}
	
}
