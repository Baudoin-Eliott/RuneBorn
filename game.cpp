#include "game.h"
#include "textureManager.h"
#include "gameMap.h"
#include "components.h"
#include "vector2D.h"
#include "collision.h"

GameMap* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_FRect Game::camera = { 0, 0, 668, 640 };
float Game::zoom = 2.f;
int Game::currentSizeMapX = 0;
int Game::currentSizeMapY = 0;
int Game::currentTileSizeX = 0;
int Game::currentTileSizeY = 0;


std::vector<ColliderComponent*> Game::colliders;

Manager manager;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& cameraEntity(manager.addEntity());



Game::Game() {

}
Game::~Game() {

}

void Game::AddWall(float x, float y, float w, float h) {
	auto& wall(manager.addEntity());
	wall.addComponent<TransformComponent>(x, y, w, h, 1);
	wall.addComponent<ColliderComponent>("Wall");
}



void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
		std::cerr << "Error while initialised the SDL component.." << SDL_GetError() << std::endl;
		return;
	}

	if (SDL_CreateWindowAndRenderer(title, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0, &window, &renderer) == 0) {
		std::cerr << "SDL_CreateWindowAndRenderer failed" << SDL_GetError() << std::endl;
		return;
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	isRunning = true;

	map = new GameMap();

	//ecs implementation
	map->LoadMap("Assets/Backgrounds/Maps/map1.tmx");

	player.addComponent<TransformComponent>(200.f, 200.f, 16, 16, 1 );
	player.addComponent<SpriteComponent>("Assets/Actor/Characters/Boy/SpriteSheet.png");
	player.addComponent<ColliderComponent>("Player");
	player.addComponent<KeyBoardController>();

	cameraEntity.addComponent<CameraComponent>(width, height);
	cameraEntity.getComponent<CameraComponent>().setTarget(&player);

}

void Game::handleEvents() {

	SDL_PollEvent(&event);

	switch (event.type) {

	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}

}
void Game::update() {
	manager.refresh();
	manager.update();
	cameraEntity.update();
	for (auto cc : colliders) {
		if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)) {
			if (cc->tag == "Wall") {
				player.getComponent<TransformComponent>().velocity * -1;
			}
		}
	}

}
void Game::render() {

	SDL_RenderClear(renderer);
	map->DrawMap();
	manager.draw();
	SDL_RenderPresent(renderer);


}
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
