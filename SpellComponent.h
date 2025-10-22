#pragma once
#include "game.h"
#include "ECS.h"
#include "components.h"

class Spell;
class Character;

class SpellComponent : public Component {


public:

	TransformComponent* transform;
	Spell* spell = nullptr;



	SpellComponent(Entity* entity, const char* name, int power);
	 


};
