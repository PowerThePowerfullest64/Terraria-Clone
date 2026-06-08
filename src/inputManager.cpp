#include "inputManager.h"

#include "raylib.h"

void InputManager::Update() {
    moveLeftDown = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    moveRightDown = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP);
}