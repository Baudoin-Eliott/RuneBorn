#include "collision.h"
#include "colliderComponent.h"
#include<iostream>

bool Collision::AABB(const SDL_FRect& recA, const SDL_FRect& recB) {

	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y) {
		return true;
	}
	return false;

}


bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {

	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	else return false;
}