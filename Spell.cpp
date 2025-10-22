#include "Spell.h"
#include "SpellEffect.h"
#include "Spells.h"
#include<iostream>

Spell::Spell(Entity* entity, const char* name, int power)
{
	std::cout << "sort cree\n";
	this->entity = entity;
	this->name = name;
	this->power = power;
	if (strcmp(name, "Heal") == 0) {
		effect = new Heal(power);
		this->type = INSTANT;
		std::cout << "Heal spell created\n";
	}
	else {
		effect = nullptr;
		this->type = NONE;
	}
		
}

void Spell::Cast() {
	if (effect) effect->OnCast(entity);
}

void Spell::Update() {
	if (effect) effect->OnUpdate(entity);
}

