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

    jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
}