#pragma once

typedef enum { INSTANT, DURATION, PROJECTILE, NONE } SpellType;


class SpellEffect;
class Entity;


class Spell {

public:
	const char* name;
	int power;
	SpellType type;
	SpellEffect* effect;
	Entity* entity;
	
	Spell(Entity* entity, const char* name, int power);
	~Spell() = default;
	void Cast();
	void Update();
};