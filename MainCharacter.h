// MainCharacter.h
#pragma once
#include "Character.h"
#include "Item.h"

class MainCharacter : public Character {
public:


    void UseItem(int pocketNumber);
    void Update();
};