#include "inputManager.hpp"

#include <iostream>

#include "raylib.h"

InputManager::InputManager() {
    std::cout << "Construced InputManager!\n";
}

InputManager::~InputManager() {
    std::cout << "Deconstruced InputManager!\n";
}

void InputManager::Update() {
    moveLeftDown = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    moveRightDown = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    jumpPressed = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    zoomInPressed = GetMouseWheelMove() < 0.f;
    zoomOutPressed = GetMouseWheelMove() > 0.f;
}