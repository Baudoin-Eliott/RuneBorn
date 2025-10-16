#pragma once
#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>
#include<iostream>
#include<vector>

class ColliderComponent;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();

	bool running() { return isRunning;};
	void render();
	void clean();

	static void AddTile(int id, int x, int y);

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static SDL_FRect camera;
	static float zoom;
	static int currentSizeMapX;
	static int currentSizeMapY;
	static int currentTileSizeX;
	static int currentTileSizeY;

private:
	int count = 0;
	bool isRunning;
	SDL_Window* window;
};