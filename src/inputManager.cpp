#include "inputManager.hpp"

#include <iostream>

#include "raylib.h"

bool InputManager::moveLeftDown = false;
bool InputManager::moveRightDown = false;
bool InputManager::jumpDown = false;

bool InputManager::zoomInPressed = false;
bool InputManager::zoomOutPressed = false;

bool InputManager::togglePausedReleased = false;

void InputManager::Update() {
    moveLeftDown = IsKeyDown(KEY_A);
    moveRightDown = IsKeyDown(KEY_D);
    jumpDown = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W);

    zoomInPressed = IsKeyPressed(KEY_UP) || GetMouseWheelMove() < -0.05f;
    zoomOutPressed = IsKeyPressed(KEY_DOWN) || GetMouseWheelMove() > 0.05f;

    togglePausedReleased = IsKeyReleased(KEY_ESCAPE);
}