#pragma once
#include "components.h"
#include "SDL3/SDL.h"

class SpriteComponent : public Component {

private:
	TransformComponent* transform;
	SDL_Texture* texture;

	SDL_FRect srcRect, destRect;
	const int frameDelay = 150;

	int frameStart;


public:

	SpriteComponent() = default;

	SpriteComponent(const char* path) {

		setTex(path);
	}
	~SpriteComponent() {

		SDL_DestroyTexture(texture);
	}


	void setTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();
		frameStart = SDL_GetTicks();
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;


	}

	void update() override {

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override {

		SDL_FRect cameraRect = destRect;

		cameraRect.x = (destRect.x - Game::camera.x);
		cameraRect.y = (destRect.y - Game::camera.y);
		cameraRect.w = destRect.w * Game::zoom;
		cameraRect.h = destRect.h * Game::zoom;
		if (SDL_GetTicks() - frameStart >= frameDelay) {
			frameStart = SDL_GetTicks();
			nextTex();
		}
		TextureManager::Draw(texture, srcRect, cameraRect);
	}

	void nextTex() {

		if (transform->state == "walk") {
			srcRect.x = transform->side * transform->width;
			srcRect.y = 32 + transform->frame * transform->height;
			transform->frame = (transform->frame + 1) % 3;
		}
		else if (transform->state == "attack") {
			transform->frame = 4;
			srcRect.x = transform->side * transform->width;
			srcRect.y = transform->frame * transform->height;
		}
		else if (transform->state == "jump") {
			transform->frame = 5;
			srcRect.x = transform->side * transform->width;
			srcRect.y = transform->frame * transform->height;
			transform->state = "idle";
		}
		else if (transform->state == "hit") {
			transform->frame = 6;
			srcRect.x = transform->width;
			srcRect.y = transform->frame * transform->height;
			transform->state = "idle";
		}
		else if (transform->state == "dead") {
			transform->frame = 6;
			srcRect.x = 0;
			srcRect.y = transform->frame * transform->height;
		}
		else if (transform->state == "sp1") {
			transform->frame = 6;
			srcRect.x = 2 * transform->width;
			srcRect.y = transform->frame * transform->height;
		}
		else if (transform->state == "sp2") {
			transform->frame = 6;
			srcRect.x = 3 * transform->width;
			srcRect.y = transform->frame * transform->height;
		}
		else {
			transform->frame = 0;
			srcRect.x = transform->side * transform->width;
			srcRect.y = 0;
		}
	}
};