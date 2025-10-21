// ItemEffect.h
#pragma once
class Character; // Forward declaration

class ItemEffect {
public:
    virtual void OnUse(Character* character) {}
    virtual void OnEquip(Character* character) {}
    virtual void OnUnequip(Character* character) {}
    virtual void OnUpdate(Character* character) {}
    virtual ~ItemEffect() = default;
};