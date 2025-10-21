#pragma once
typedef enum { CONSUMABLE, HELMET, CHEST, PANTS, BOOTS, MISC } ItemType;

class ItemEffect;
class Character; // Forward declaration

class Item {
public:
    ItemType type;
    const char* name;
    const char* description;
    ItemEffect* effect;

    void OnUse(Character* character);
    void OnEquip(Character* character);
    void OnUnequip(Character* character);
};