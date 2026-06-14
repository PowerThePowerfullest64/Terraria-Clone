#include "inputManager.hpp"

#include <iostream>

#include "raylib.h"

bool InputManager::moveLeftDown = false;
bool InputManager::moveRightDown = false;

bool InputManager::jumpPressed = false;
bool InputManager::zoomInPressed = false;
bool InputManager::zoomOutPressed = false;

void InputManager::Update() {
    moveLeftDown = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    moveRightDown = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    jumpPressed = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    zoomInPressed = GetMouseWheelMove() < 0.f;
    zoomOutPressed = GetMouseWheelMove() > 0.f;
}