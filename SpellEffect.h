#pragma once
class Entity;
class SpellEffect {
public:

    virtual void OnCast(Entity* spellEntity) {}
    virtual void OnUpdate(Entity* spellEntity) {}
	virtual void OnHit(Entity* targetEntity) {}
    virtual ~SpellEffect() = default;
};

