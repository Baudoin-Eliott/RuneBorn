#include "FireBall.h"



FireBall::FireBall(Entity* speelEntity, int pow)
{
	power = pow;

	TransformComponent transComp = speelEntity->getComponent<TransformComponent>();
	Character chara = speelEntity->getComponent<Character>();
	side = transComp.side;
	transform->position = transComp.position;
	switch (side) {
	case(0):
		transform->velocity = Vector2D(0, speed);
		break;

	case(1):
		transform->velocity = Vector2D(0, -speed);
		break;
	case(2):
		transform->velocity = Vector2D(-speed, 0);
		break;
	case(3):
		transform->velocity = Vector2D(speed, 0);
		break;

	}
}

void FireBall::OnHit(Entity* speelEntity)
{
	speelEntity->getComponent<Character>().TakeDamage(damage * (power / 100.f));
	speelEntity->destroy();
}
