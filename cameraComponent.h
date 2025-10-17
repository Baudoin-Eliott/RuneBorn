#pragma once
#include "ECS.h"
#include "components.h"
#include "SDL3/SDL.h"

class CameraComponent : public Component {

public:
    SDL_FRect camera;
    Entity* target;

    CameraComponent() {
        camera.x = 0;
        camera.y = 0;
        camera.w = 668;
        camera.h = 640;
        target = nullptr;
    }

    CameraComponent(int w, int h) {
        camera.x = 0;
        camera.y = 0;
        camera.w = w;
        camera.h = h;
        target = nullptr;
    }

    void init() override {
        // Initialise la position de la caméra au spawn
        if (target != nullptr && target->hasComponent<TransformComponent>()) {
            TransformComponent* targetTransform = &target->getComponent<TransformComponent>();
            camera.x = targetTransform->position.x - (camera.w / 2.0f);
            camera.y = targetTransform->position.y - (camera.h / 2.0f);
        }
    }

    void update() override {
        if (target != nullptr && target->hasComponent<TransformComponent>()) {
            TransformComponent* targetTransform = &target->getComponent<TransformComponent>();


            float targetCamX = targetTransform->position.x - (camera.w / 2.0f);
            float targetCamY = targetTransform->position.y - (camera.h / 2.0f);

            camera.x = targetCamX;
            camera.y = targetCamY;
            
            int mapSizeX = Game::currentSizeMapX * Game::currentTileSizeX - Game::camera.w;
            int mapSizeY = Game::currentSizeMapY * Game::currentTileSizeY - Game::camera.h;


            if (camera.x >= 0) {
                if (camera.x <= mapSizeX) Game::camera.x = camera.x;
                else Game::camera.x = mapSizeX;
            }
            else Game::camera.x = 0;

            if (camera.y >= 0) {
                if (camera.y <= mapSizeY) Game::camera.y = camera.y;
                else Game::camera.y = mapSizeY;
            }
            else Game::camera.y = 0;

            

        }
    }

    void setTarget(Entity* newTarget) {
        target = newTarget;
    }
};