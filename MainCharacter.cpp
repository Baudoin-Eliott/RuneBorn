#include "MainCharacter.h"
#include "ItemEffect.h"

void MainCharacter::UseItem(int pocketNumber) {
    Item* item = (pocketNumber == 1) ? pocket1 : pocket2;
    if (item) item->OnUse(this); // this = MainCharacter* qui hérite de Character*
}

void MainCharacter::Update() {

    if (pocket1) pocket1->effect->OnUpdate(this);
   	if (pocket2) pocket2->effect->OnUpdate(this);
    if (helmet) helmet->effect->OnUpdate(this);
    if (chest) chest->effect->OnUpdate(this);
    if (pants) pants->effect->OnUpdate(this);
    if (boots) boots->effect->OnUpdate(this);
}