#pragma once
// Enemy.h
#pragma once
#include "Character.h"

class Enemy : public Character {
public:
    // Spécifique aux ennemis
    int attackPower;
    void Attack(Character* target);
};