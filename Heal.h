#pragma once
#include "SpellEffect.h"



class Heal : public SpellEffect {
public:
	int pvRegen = 100;
	int power;
	Heal(int power);
	void OnCast(Entity* spellEntity) override;

	


};