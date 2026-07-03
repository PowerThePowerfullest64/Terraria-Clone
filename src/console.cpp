#include "console.hpp"

#include <iostream>

#include "raylib.h"

#include "inputManager.hpp"

void Console::Update() {
    if (InputManager::consoleOpenReleased && !open) {
        Open();

        return;
    }

    // Don't detect console related inputs if it isn't opened.
    if (!open)
        return;

    if (IsKeyReleased(KEY_BACKSPACE)) {
        if (input.size() > 0) {
            input.pop_back();
            std::cout << input << "\n";
        }
        
        return;
    }
    else if (IsKeyReleased(KEY_ESCAPE)) {
        Close();

        return;
    }
    else if (IsKeyReleased(KEY_ENTER)) {
        if (input.size() == 0) {
            Close();
            return;
        }

        ExecuteCommand(input);

        Close();
    }

    int key = GetCharPressed();
    if (key != 0) {
        input += static_cast<char>(key);
        std::cout << input << "\n";
    }
}

void Console::ExecuteCommand(const std::string& command) {
    std::cout << "Executed command with name '" << command << "'!\n";
}