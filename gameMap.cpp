#include "gameMap.h"
#include "textureManager.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>

GameMap::GameMap() {
    src.x = src.y = 0;
    src.w = src.h = 32;
    dest.w = dest.h = 32;
    dest.x = dest.y = 0;
}

GameMap::~GameMap() {
    for (auto& ts : tilesets) {
        if (ts.texture) SDL_DestroyTexture(ts.texture);
    }
}

Tileset* GameMap::GetTilesetForGID(int gid) {
    Tileset* last = nullptr;
    for (auto& ts : tilesets) {
        if (gid >= ts.firstGID) last = &ts;
    }
    return last; // retourne le tileset correspondant au GID
}

void GameMap::LoadMap(const char* path) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Impossible de charger le fichier TMX : " << path << std::endl;
        return;
    }

    tinyxml2::XMLElement* mapElem = doc.FirstChildElement("map");
    if (!mapElem) {
        std::cerr << "TMX invalide : pas d'élément <map>" << std::endl;
        return;
    }

    // Charger les tilesets
    for (tinyxml2::XMLElement* tsElem = mapElem->FirstChildElement("tileset"); tsElem; tsElem = tsElem->NextSiblingElement("tileset")) {
        std::string pathTSX = std::string("Assets/Backgrounds/Tiles/") + tsElem->Attribute("source");
        const char* sourceTSX = pathTSX.c_str();

        tinyxml2::XMLDocument tsxDoc;

        tinyxml2::XMLElement* tsXLMElem = nullptr;

        if (sourceTSX) {
            // Charger le TSX externe
            if (tsxDoc.LoadFile(sourceTSX) != tinyxml2::XML_SUCCESS) {
                std::cerr << "Impossible de charger le TSX : " << sourceTSX << std::endl;
                continue;
            }
            tsXLMElem = tsxDoc.FirstChildElement("tileset");
        }
        else {
            // Tileset interne dans le TMX
            tsXLMElem = tsElem;
        }

        if (!tsXLMElem) {
            std::cerr << "Tileset invalide !" << std::endl;
            continue;
        }

        Tileset ts;
        ts.firstGID = tsElem->IntAttribute("firstgid");
        tsXLMElem->QueryIntAttribute("tilewidth", &ts.tileWidth);
        tsXLMElem->QueryIntAttribute("tileheight", &ts.tileHeight);
        tsXLMElem->QueryIntAttribute("tilecount", &ts.tileCount);
        tsXLMElem->QueryIntAttribute("columns", &ts.columns);
        Game::currentTileSizeX = ts.tileWidth;
        Game::currentTileSizeY = ts.tileHeight;

        tinyxml2::XMLElement* imageElem = tsXLMElem->FirstChildElement("image");
        if (!imageElem || !imageElem->Attribute("source")) {
            std::cerr << "Tileset sans image valide !" << std::endl;
            continue;
        }

        std::stringstream ss;
        ss << "Assets/Backgrounds/Tiles/" << imageElem->Attribute("source");
        ts.texture = TextureManager::LoadTexture(ss.str().c_str());

        if (!ts.texture) {
            std::cerr << "Impossible de charger la texture : " << imageElem->Attribute("source") << std::endl;
            continue;
        }

        tilesets.push_back(ts);
    }


    // Charger les layers
    for (tinyxml2::XMLElement* layerElem = mapElem->FirstChildElement("layer"); layerElem; layerElem = layerElem->NextSiblingElement("layer")) {
        Layer layer;
        const char* layerName = layerElem->Attribute("name");
        if (!layerName) {
            std::cerr << "Layer sans nom, ignoré" << std::endl;
            continue;
        }
        layer.name = layerName;

        layer.width = layerElem->IntAttribute("width");
        layer.height = layerElem->IntAttribute("height");
        Game::currentSizeMapX = layer.width;
        Game::currentSizeMapY = layer.height;

        tinyxml2::XMLElement* dataElem = layerElem->FirstChildElement("data");
        if (!dataElem) {
            std::cerr << "Layer " << layer.name << " sans <data>, ignoré" << std::endl;
            continue;
        }

        const char* csv = dataElem->GetText();
        if (!csv) {
            std::cerr << "Layer " << layer.name << " vide, ignoré" << std::endl;
            continue;
        }

        std::string s = csv;
        size_t pos = 0;
        while ((pos = s.find(',')) != std::string::npos) {
            std::string token = s.substr(0, pos);
            std::stringstream ss(token);
            int value;
            ss >> value;
            layer.tiles.push_back(value);
            s.erase(0, pos + 1);
        }
        if (!s.empty()) {
            std::stringstream ss(s);
            int value;
            ss >> value;
            layer.tiles.push_back(value);
        }

        layers.push_back(layer);
        std::cout << "Layer " << layer.name << " loaded avec " << layer.tiles.size() << " tiles." << std::endl;
    
    }

    for (tinyxml2::XMLElement* objElem = mapElem->FirstChildElement("objectgroup"); objElem; objElem = objElem->NextSiblingElement("objectgroup")) {
		if (objElem->Attribute("name") && std::string(objElem->Attribute("name")) == "collisions") {
			for (tinyxml2::XMLElement* object = objElem->FirstChildElement("object"); object; object = object->NextSiblingElement("object")) {
				int x = object->IntAttribute("x");
				int y = object->IntAttribute("y");
				int width = object->IntAttribute("width");
				int height = object->IntAttribute("height");


				Game::AddWall(x, y, width, height);
				std::cout << "Collision box at (" << x << ", " << y << ") size (" << width << "x" << height << ")" << std::endl;
			}
		}





    std::cout << "Chargement TMX terminé avec " << tilesets.size() << " tilesets et "
        << layers.size() << " layers." << std::endl;
}


void GameMap::DrawMap() {
    for (const Layer& layer : layers) {
        for (int row = 0; row < layer.height; ++row) {
            for (int col = 0; col < layer.width; ++col) {
                int gid = layer.tiles[row * layer.width + col];
                if (gid == 0) continue; // 0 = tile vide

                Tileset* ts = GetTilesetForGID(gid);
                if (!ts) continue;

                int localID = gid - ts->firstGID;
                int tx = (localID % ts->columns) * ts->tileWidth;
                int ty = (localID / ts->columns) * ts->tileHeight;

                src.x = tx;
                src.y = ty;
                src.w = ts->tileWidth;
                src.h = ts->tileHeight;

                dest.x = (col * ts->tileWidth - Game::camera.x) * Game::zoom;
                dest.y = (row * ts->tileHeight - Game::camera.y) * Game::zoom;
                dest.w = ts->tileWidth * Game::zoom;
                dest.h = ts->tileHeight * Game::zoom;

                TextureManager::Draw(ts->texture, src, dest);
            }
        }
    }
}
