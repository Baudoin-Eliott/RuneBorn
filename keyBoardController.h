#pragma once

#include "game.h"
#include "ECS.h"
#include "components.h"

class KeyBoardController : public Component {

public:
	TransformComponent* transform;

	void init() override {

		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {

		if (Game::event.type == SDL_EVENT_KEY_DOWN) {

			switch (Game::event.key.key)
			{
			case (SDLK_Z):
				if (transform->velocity.x != 0) {
					transform->velocity.x = sqrt(1) * transform->velocity.x;
					transform->velocity.y = -sqrt(1);
					break;
				}
				transform->velocity.y = -1;
				break;

			case (SDLK_S):
				if (transform->velocity.x != 0) {
					transform->velocity.x = sqrt(1) * transform->velocity.x;
					transform->velocity.y = sqrt(1);
					break;
				}
				transform->velocity.y = 1;
				break;
			case (SDLK_D):
				if (transform->velocity.y != 0) {
					transform->velocity.y = sqrt(1) * transform->velocity.y;
					transform->velocity.x = sqrt(1);
					break;
				}
				transform->velocity.x = 1;
				break;
			case (SDLK_Q):
				if (transform->velocity.y != 0) {
					transform->velocity.y = sqrt(1) * transform->velocity.y;
					transform->velocity.x = -sqrt(1);
					break;
				}
				transform->velocity.x = -1;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_EVENT_KEY_UP) {

			switch (Game::event.key.key)
			{
			case (SDLK_Z):
				transform->velocity.y = 0;
				break;

			case (SDLK_S):
				transform->velocity.y = 0;
				break;
			case (SDLK_D):
				transform->velocity.x = 0;
				break;
			case (SDLK_Q):
				transform->velocity.x = 0;
				break;
			default:
				break;
			}
		}



	}
};