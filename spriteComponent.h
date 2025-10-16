#pragma once
#include "components.h"
#include "SDL3/SDL.h"

class SpriteComponent : public Component {

private:
	TransformComponent* transform;
	SDL_Texture* texture;

	SDL_FRect srcRect, destRect;


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
		cameraRect.w = destRect.w ;
		cameraRect.h = destRect.h;

		TextureManager::Draw(texture, srcRect, cameraRect);
	}






















































};