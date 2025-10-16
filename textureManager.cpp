
#include "textureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {

	SDL_Surface* tempSurface = IMG_Load(fileName);
	if (!tempSurface) std::cout << "L'image n'a pas put etre charger, fichier introuvable a: " << fileName << std::endl;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_DestroySurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_FRect src, SDL_FRect dest)
{
	SDL_RenderTexture(Game::renderer, tex, &src, &dest);
}
