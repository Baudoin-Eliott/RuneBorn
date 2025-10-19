#include <algorithm>
#include "game.h"
#include "textureManager.h"
#include "gameMap.h"
#include "components.h"
#include "vector2D.h"
#include "collision.h"

GameMap* map;

RuneSystem Game::runeSystem;
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
	map->LoadMap("Assets/Backgrounds/Maps/map1.tmx", 3);

	player.addComponent<TransformComponent>(500.f, 500.f, 16, 16, 1);
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

	bool collided = false;
	SDL_FRect a = player.getComponent<TransformComponent>().newPos;
	SDL_FRect playerRect = { a.x + a.w / 5, a.y + a.h, a.w * 2 - (a.w / 5) * 2, a.h };

	for (auto cc : colliders) {
		SDL_FRect rect = { cc->collider.x * zoom, cc->collider.y * zoom, cc->collider.w, cc->collider.h };
		if (Collision::AABB(playerRect, rect)) {
			if (cc->tag == "Wall") {
				collided = true;
			}
		}
	}

	// Détection de nouveaux points pendant le tracé
	if (runeSystem.isDrawing && !runeSystem.connections.empty()) {
		int closestPoint = runeSystem.FindClosestPoint(Vector2D(
			event.motion.x,  // MOTION !!!!
			event.motion.y
		));

		if (closestPoint != -1){
			if (std::find(runeSystem.connections.begin(),
				runeSystem.connections.end(),
				closestPoint) == runeSystem.connections.end()) {
					runeSystem.connections.push_back(closestPoint);
			}
			if (closestPoint == runeSystem.connections[0] && runeSystem.connections.size() > 2) {
				runeSystem.connections.push_back(closestPoint);
				runeSystem.ComparePattern(runeSystem.closeRuneMenu());
				std::cout << "Pattern tracé avec " << runeSystem.connections.size() << " points" << std::endl;
			}
		}
	}

	if (!collided) player.getComponent<TransformComponent>().NewPos();
}
void Game::render() {

	SDL_RenderClear(renderer);
	map->DrawMap_Bottom();
	manager.draw();
	map->DrawMap_Up();
	if (runeSystem.isCasting) {
		runeSystem.DrawRuneMenu();
	}
	SDL_RenderPresent(renderer);

}
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
