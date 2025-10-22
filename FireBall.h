#pragma once
#include "SpellEffect.h"
#include "components.h"

class FireBall : public SpellEffect
{
	SpriteComponent* sprite;
	TransformComponent* transform;
	
	//caracteristiques du sort
	int power;
	int damage = 1;
	int speed = 2; 
	int side;

	//suppression
	int creationtime;
	int lifetime = 600;


	FireBall(int pow);

	FireBall(Entity* speelEntity, int pow);

	void OnHit(Entity* speelEntity) override;

	~FireBall() = default;

};

