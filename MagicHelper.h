#pragma once
#include "ItemEffect.h"
#include <vector>

class MagicHelperEffect : public ItemEffect {
public:
    std::vector<int> pattern;
    int lvl;

    MagicHelperEffect(const char* name, int lvl);

    void OnEquip(Character* character) override;
    void OnUnequip(Character* character) override;
};