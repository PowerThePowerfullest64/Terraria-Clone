#pragma once

class InputManager {
public:
    InputManager();
    ~InputManager();

    bool moveLeftDown = false;
    bool moveRightDown = false;

    bool jumpPressed = false;
    bool zoomInPressed = false;
    bool zoomOutPressed = false;

    void Update();
};