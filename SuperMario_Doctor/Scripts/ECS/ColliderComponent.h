#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"


class ColliderComponent : public Component {

public:

	SDL_Rect collider;

	//makes a destinction between the objects when they collide with the player 
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t) {

		tag = t;
	}

	// sets the transform component to that of the enetity, so it can be used in the update method
	void init() override {

		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		//adds a collider to the array of colliders 
		Game::colliders.push_back(this);
	}


	// sets the collider positions and size to those taken from the transform component
	void update() override {


		collider.x = static_cast<int>(transform->getX());
		collider.y = static_cast<int>(transform->getY());
		collider.w = entity->wSize;
		collider.h = entity->hSize;
	}

};