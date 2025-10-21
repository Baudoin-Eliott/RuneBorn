#include "Item.h"
#include "ItemEffect.h"

void Item::OnUse(Character* character) {
    if (effect) effect->OnUse(character);
}

void Item::OnEquip(Character* character) {
    if (effect) effect->OnEquip(character);
}

void Item::OnUnequip(Character* character) {
    if (effect) effect->OnUnequip(character);
}