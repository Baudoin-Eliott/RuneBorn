#include "Spell.h"
#include "SpellComponent.h"


SpellComponent::SpellComponent(Entity* entity, const char* name, int power) {
	this->spell = new Spell(entity, name, power);
	std::cout << spell->type;
	if (spell->type == INSTANT) {
		std::cout << "Casting instant spell";
		spell->Cast();
	}
}