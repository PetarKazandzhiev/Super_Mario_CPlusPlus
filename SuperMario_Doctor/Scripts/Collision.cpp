#include "Collision.h"
#include "ECS\ColliderComponent.h"

//makes the collision between 2 rectangles
bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {

	if (recA.x + recA.w >= recB.x && 
		recB.x + recB.w >= recA.x &&
		recA.y +recA.h >= recB.y &&
		recB.y + recB.h >= recA.y)
	{
		return true;
	}

	return false;
}

//this thakes the collider components of the 2 enities
bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {

	if (AABB(colA.collider, colB.collider))
	{
		return true;
	}
	else
	{
		return false;
	}

}