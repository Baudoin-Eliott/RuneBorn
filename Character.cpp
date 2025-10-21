#include "Character.h"
#include "Item.h" 

void Character::PutItemInPocket(Item* item, int pocketNumber) {
    if (pocketNumber == 1) {
        pocket1 = item;
        pocket1->OnEquip(this);
    }
    else if (pocketNumber == 2) {
        pocket2 = item;
		pocket2->OnEquip(this);
    }
}

void Character::EquipArmor(Item* item) {
    switch (item->type) {
    case HELMET: helmet = item; break;
    case CHEST: chest = item; break;
    case PANTS: pants = item; break;
    case BOOTS: boots = item; break;
    default: break;
    }
}