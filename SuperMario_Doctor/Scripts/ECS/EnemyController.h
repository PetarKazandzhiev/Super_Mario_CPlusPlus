#pragma once
#include "ECS.h"
#include "Components.h"

class EnemyController : public Component {
private:
	int direction = 1;

	/*changes the direction,  flips the sprite*/
	void ChangeDirection() {
		direction *= -1;
		transform->velocity.setX(direction);
		if (direction == 1) {
			sprite->spriteFlip = SDL_FLIP_NONE;
		}
		else {
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
	}

public:
	EnemyController() {
		//returns a number between 1 and 100 at random and changes the direction based on that
		int dice = rand() % 100 + 1;
		if (dice > 50) {
			direction = -1;
		}
	}

	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		//gives it a transform component
		transform = &entity->getComponent<TransformComponent>();
		//gives it a sprite component
		sprite = &entity->getComponent<SpriteComponent>();
		//changes direction to set a value to the velocity of the object
		ChangeDirection();
		//plays animation
		sprite->Play("Walk");
	}

	void update() override
	{
		//checks if the entity is going to hit a wall with its velocity 
		if (!transform->collisionCheck(transform->position.getX() + transform->velocity.getX() * entity->speed,
				transform->position.getY()) ||
			//checks if the entity is going to be over a hole
			transform->collisionCheck(
				(transform->position.getX() + transform->velocity.getX() * entity->speed) + transform->velocity.getX() * entity->wSize,
				transform->position.getY() + 8)) {
			ChangeDirection();
		}
	}
};