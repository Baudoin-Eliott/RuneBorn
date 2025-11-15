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
				transform->side = 1;
				transform->state = "walk";
				transform->velocity.x = 0;
				transform->velocity.y = -1;

				break;

			case (SDLK_S):

				transform->side = 0;
				transform->state = "walk";
				transform->velocity.x = 0;
				transform->velocity.y = 1;
				break;
			case (SDLK_D):
				transform->side = 3;
				transform->state = "walk";
				transform->velocity.y = 0;
				transform->velocity.x = 1;
				break;
			case (SDLK_Q):
				transform->side = 2;
				transform->state = "walk";
				transform->velocity.y = 0;
				transform->velocity.x = -1;
				break;
			case(SDLK_E):
				if (Game::runeSystem.isCasting == false) Game::runeSystem.openRuneMenu(9);
				else Game::runeSystem.closeRuneMenu();
			case(SDLK_A):

			default:
				break;
			}
		}
		if (Game::event.type == SDL_EVENT_KEY_UP) {
			switch (Game::event.key.key)
			{
			case (SDLK_Z):
				transform->velocity.y = 0;
				transform->state = "idle";
				break;

			case (SDLK_S):
				transform->velocity.y = 0;
				transform->state = "idle";
				break;
			case (SDLK_D):
				transform->velocity.x = 0;
				transform->state = "idle";
				break;
			case (SDLK_Q):
				transform->velocity.x = 0;
				transform->state = "idle";
				break;
			default:
				break;
			}

		}
		if (Game::event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			switch (Game::event.button.button) {

			case(SDL_BUTTON_LEFT):
				if (Game::runeSystem.isCasting) {
					int closestPoint = Game::runeSystem.FindClosestPoint(
						Vector2D(Game::event.button.x, Game::event.button.y)
					);

					if (closestPoint != -1) {
						Game::runeSystem.isDrawing = true;
						Game::runeSystem.connections.push_back(closestPoint);
					}
				}
				break;
			}
		}
		if (Game::event.type == SDL_EVENT_MOUSE_BUTTON_UP){
			switch (Game::event.button.button) {
			case (SDL_BUTTON_LEFT):
				if (Game::runeSystem.isCasting && !Game::runeSystem.connections.empty()) {
					Game::runeSystem.ComparePattern(Game::runeSystem.closeRuneMenu());
				}
				break;
			}
		}
	}
};