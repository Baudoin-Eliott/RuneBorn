
#include "Heal.h"
#include "Character.h"


Heal::Heal(int pow) {
	power = pow;
}

void Heal::OnCast(Entity* spellEntity) {
	Character& chara = spellEntity->getComponent<Character>();
	if (&chara == nullptr)
		return;
	chara.health += pvRegen * (power / 100.f);
	if (chara.health > chara.maxHealth) {
		chara.health = chara.maxHealth;
	}
	std::cout << "on l'a heal";
}