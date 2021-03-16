#pragma once
#include "../Map.h"

class TransformComponent : public Component
{
private:
	Map* map;
	int jumpSpeed = 0;
	int currentGravityExaleration = 0;

public:
	Vector2D position;
	Vector2D velocity;

	int speed = 3;
	bool gravityAffected = true;

	int gravityForce = 1;
	int weight = 32;

	TransformComponent(Map* mapInput, float x = 0, float y = 0, int inputWeight = 32)
	{
		map = mapInput;
		setPosition(x, y);
		weight = inputWeight;
	}

	
	TransformComponent()
	{
		//sets the position to zero
		position.Zero();
	}

	float getX() { return position.getX(); }
	float getY() { return position.getY(); }

	TransformComponent(float x, float y)
	{
		setPosition(x, y);
	}

	/*returns true and sets the position if the entity will not collide with anything*/
	bool setPosition(float x, float y)
	{
		if (collisionCheck(x, y)) {
			position.setX(x);
			position.setY(y);
			return true;
		}
		return false;
	}

	/*returns the opposite of the collisionCheck method in Map.cpp.
	If the map collisioncheck returns true ,you have not collided ,
	if it returns false , you have collided */
	bool collisionCheck(float x, float y) {
		if (entity == nullptr){
			return true;
		}
		if (map == nullptr) {
			return true;
		}
		int xStart = (int)x;
		int xEnd = (int)x + entity->wSize;
		int yStart = (int)y;
		int yEnd = (int)y + entity->hSize;

		return !map->collisionCheck(xStart, xEnd, yStart, yEnd);
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		//makes the target moove left and right
		setPosition(position.getX() + velocity.getX() * entity->speed, position.getY());
		//if you hit something(the cieling) the jump speed is set to 0
		if (!setPosition(position.getX(), position.getY() - jumpSpeed)) {
			jumpSpeed = 0;
		}
		//if you have jumped the jumpspeed decreases until it reaches 0
		if (jumpSpeed > 0){
			jumpSpeed--;
		}
		else{
			applyGravity();
		}
		

	}

	//applies gravity (if you are already affected by the gravity it terminates the function)
	void applyGravity()
	{
		if (!gravityAffected) {
			return;
		}

		if (setPosition(position.getX(), position.getY() + gravityForce + currentGravityExaleration))
		{
			currentGravityExaleration += weight / 32;
		}
		else
		{
			currentGravityExaleration = 0;
		}
	}

	//checks if you are in the air and if you are not it sets the jump speed
	void Jump() {
		if (!collisionCheck(position.getX(), position.getY() + gravityForce)){
			jumpSpeed = entity->jumpSpeed;

		}
		
	}
};