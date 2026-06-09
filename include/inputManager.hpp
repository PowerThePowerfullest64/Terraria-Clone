#pragma once

class InputManager {
public:
    InputManager();
    ~InputManager();

    bool moveLeftDown = false;
    bool moveRightDown = false;

    bool jumpPressed = false;

    void Update();
};