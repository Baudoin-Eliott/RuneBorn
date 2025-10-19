#include "runeSystem.h"
#include "textureManager.h"
#include <iostream>
#include <cmath>

RuneSystem::RuneSystem() {
    isCasting = false;
    isDrawing = false;
    difficulty = 0;
    spacingX = 0;
    spacingY = 0;
    tolerance = 20.0f;
    frame = 0;
    frameStart = 0;
    frameDelay = 100;
}

RuneSystem::~RuneSystem() {
}

void RuneSystem::openRuneMenu(int dif) {
    isCasting = true;
    isDrawing = false;
    difficulty = dif;
    connections.clear();  // Reset les connexions
}

std::vector<int> RuneSystem::closeRuneMenu() {
    isCasting = false;
    isDrawing = false;
    return connections;
}

void RuneSystem::DrawRuneMenu() {
    SDL_Texture* StarTex = TextureManager::LoadTexture("Assets/FX/Magic/Circle/SpriteSheetSpark2.png");
    drawBack();

    spacingX = 668.0f / (difficulty + 1);
    spacingY = 640.0f / (difficulty + 1);
    float StartSize = (spacingX - 8.0f) < 40 ? (spacingX - 8.0f) : 40;

    // Animation des étoiles (optionnel)
    /*
    if (SDL_GetTicks() - frameStart >= frameDelay) {
        frameStart = SDL_GetTicks();
        frame = (frame + 1) % 4;
    }
    */
    

    // Dessine les points de la grille
    for (int i = 0; i < difficulty; i++) {
        for (int j = 0; j < difficulty; j++) {
            TextureManager::Draw(
                StarTex,
                { (float)frame * 32, 0, 32, 32 },
                { (i + 1) * spacingX - StartSize / 2,
                 (j + 1) * spacingY - StartSize / 2,
                 StartSize,
                 StartSize }
            );
        }
    }

    // Dessine les lignes entre points connectés
    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);  // Jaune
    for (int i = 0; i < (int)connections.size() - 1; i++) {
        Tuple2f start = getPointPosition(connections[i]);
        Tuple2f end = getPointPosition(connections[i + 1]);
        SDL_RenderLine(Game::renderer, start.x, start.y, end.x, end.y);
    }

    // Si on dessine actuellement, ligne vers la souris
     SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
     if (isDrawing && !connections.empty()) {
         Tuple2f lastPoint = getPointPosition(connections.back());
         if ((float)Game::event.motion.x != 0.f && (float)Game::event.motion.y != 0.f)
         {
             lastMousePos = Tuple2f{
                 (float)Game::event.motion.x,
                 (float)Game::event.motion.y
             };
             Tuple2f mousePos = lastMousePos;
             SDL_RenderLine(Game::renderer, lastPoint.x, lastPoint.y, mousePos.x, mousePos.y);
         }
     }
}

void RuneSystem::drawBack() {
    SDL_Texture* BackTex = TextureManager::LoadTexture("Assets/Ui/Dialog/FacesetBox.png");

    // Draw the Left upper corner
    TextureManager::Draw(BackTex, { 0, 0, 16, 16 }, { 20, 20, 16, 16 });
    // Draw the Right upper corner
    TextureManager::Draw(BackTex, { 32, 0, 16, 16 }, { 632, 20, 16, 16 });
    // Draw the Left lower corner
    TextureManager::Draw(BackTex, { 0, 32, 16, 16 }, { 20, 604, 16, 16 });
    // Draw the Right lower corner
    TextureManager::Draw(BackTex, { 32, 32, 16, 16 }, { 632, 604, 16, 16 });
    // Draw the Upper border
    TextureManager::Draw(BackTex, { 16, 0, 16, 16 }, { 36, 20, 608, 16 });
    // Draw the Lower border
    TextureManager::Draw(BackTex, { 16, 32, 16, 16 }, { 36, 604, 608, 16 });
    // Draw the Left border
    TextureManager::Draw(BackTex, { 0, 16, 16, 16 }, { 20, 32, 16, 575 });
    // Draw the Right border
    TextureManager::Draw(BackTex, { 32, 16, 16, 16 }, { 632, 32, 16, 575 });
    // Fill the center
    TextureManager::Draw(BackTex, { 16, 16, 16, 16 }, { 36, 36, 604, 575 });
}

void RuneSystem::ComparePattern(std::vector<int> userPattern)
{








}

int RuneSystem::FindClosestPoint(Vector2D screenPos) {
    // screenPos est en coordonnées écran (0-668, 0-640)
    for (int i = 0; i < difficulty; i++) {
        for (int j = 0; j < difficulty; j++) {
            float posX = (i + 1) * spacingX;
            float posY = (j + 1) * spacingY;
            float dist = sqrt((screenPos.x - posX) * (screenPos.x - posX) +
                (screenPos.y - posY) * (screenPos.y - posY));
            if (dist < tolerance) {
                return i + j * difficulty;
            }
        }
    }
    return -1;
}

Tuple2f RuneSystem::getPointPosition(int pointId) {
    float x = (pointId % difficulty + 1) * spacingX;
    float y = (pointId / difficulty + 1) * spacingY;
    Tuple2f pos = { x, y };
    return pos;
}