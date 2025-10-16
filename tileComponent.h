#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL3/SDL.h"


class TileComponent : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_FRect tileRect;
	int tileID;
	const char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) {

		tileRect.x = x;
		tileRect.y = y;
		tileRect.h = h;
		tileRect.w = w;
		tileID = id;

		switch (tileID) {
		case(0):
			path = "Assets/Teste/water.png";
			break;
		case(1):
			path = "Assets/Teste/dirt.png";
			break;
		case(2):
			path = "Assets/Teste/grass.png";
			break;

		}


	}

	void init() override {

		entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, (float)tileRect.w, (float)tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();


	}

};
