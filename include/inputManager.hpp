#pragma once

class InputManager {
public:
    InputManager() = delete;

    static bool moveLeftDown;
    static bool moveRightDown;
    static bool jumpDown;
    
    static bool zoomInPressed;
    static bool zoomOutPressed;

    static bool togglePausedReleased;

    static void Update();
};