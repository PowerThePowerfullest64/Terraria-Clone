#pragma once

class InputManager {
public:
    InputManager() = delete;

    static bool moveLeftDown;
    static bool moveRightDown;

    static bool jumpPressed;
    static bool zoomInPressed;
    static bool zoomOutPressed;

    static void Update();
};