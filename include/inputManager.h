#pragma once

class InputManager {
public:
    bool moveLeftDown = false;
    bool moveRightDown = false;

    bool jumpPressed = false;

    void Update();
};