#pragma once
#include "game.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* fileName);

	static void Draw(SDL_Texture* tex, SDL_FRect src, SDL_FRect dest);

};