#pragma once
#include "ECS.h"

class Item;

class Character : public Component {
public:
    int health;
    int maxHealth;
    int healthRegen;
    int mana;
    int maxMana;
    int manaRegen;

    Item* pocket1 = nullptr;
    Item* pocket2 = nullptr;
    Item* helmet = nullptr;
    Item* chest = nullptr;
    Item* pants = nullptr;
    Item* boots = nullptr;

    virtual void TakeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    virtual void Heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    void PutItemInPocket(Item* item, int pocketNumber);

    void EquipArmor(Item* item);
};