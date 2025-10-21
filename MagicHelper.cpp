#include "MagicHelper.h"
#include "game.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

MagicHelperEffect::MagicHelperEffect(const char* name, int lvl1) {
    std::ifstream file("Assets/Datas/sort.json");
    json data;
    file >> data;
    lvl = lvl1;
    std::string levelKey = "Lvl" + std::to_string(lvl);
    if (data[levelKey].is_null())
        return;
    for (auto it = data[levelKey].begin(); it != data[levelKey].end(); ++it) {
        if (it.key() == name) {
            pattern = it.value()["value"].get<std::vector<int>>();
            break;
        }
    }
}

void MagicHelperEffect::OnEquip(Character* character) {
    Game::runeSystem.helpedSort = this;
}

void MagicHelperEffect::OnUnequip(Character* character) {
    Game::runeSystem.helpedSort = nullptr;
}