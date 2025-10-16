#pragma once
#include "game.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "SDL3/SDL.h"

struct Layer {
    std::string name;
    int width;
    int height;
    std::vector<int> tiles; // GID de chaque tile
};

struct Tileset {
    int firstGID;
    int tileWidth;
    int tileHeight;
    int columns;
    int tileCount;
    SDL_Texture* texture;
};

class GameMap {
public:
    GameMap();
    ~GameMap();

    void LoadMap(const char* path);
    void DrawMap();

private:
    SDL_FRect src, dest;

    std::vector<Layer> layers;
    std::vector<Tileset> tilesets;

    Tileset* GetTilesetForGID(int gid);
};
