#pragma once
#include "SDL3/SDL.h"
#include <vector>
#include "vector2D.h"
#include "game.h"

struct Tuple2f {
    float x;
    float y;
};

class RuneSystem {
public:
    bool isCasting;
    bool isDrawing;
    int difficulty;

    std::vector<int> connections;

    float spacingX;
    float spacingY;
    float tolerance;
    Tuple2f lastMousePos;

    int frame;
    Uint32 frameStart;
    int frameDelay;

    RuneSystem();
    ~RuneSystem();

    void openRuneMenu(int dif);
    std::vector<int> closeRuneMenu();
    void DrawRuneMenu();
    void drawBack();
    static void ComparePattern(std::vector<int> userPattern);

    int FindClosestPoint(Vector2D screenPos);
    Tuple2f getPointPosition(int pointId);
};