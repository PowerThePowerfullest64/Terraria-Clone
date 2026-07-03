#include "console.hpp"

#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>

#include "raylib.h"

#include "inputManager.hpp"
#include "commands.hpp"
#include "gameplaySession.hpp"
#include "settings.hpp"

Console::Console(GameplaySession* gameplaySession) :
    gameplaySession(gameplaySession) {

    // Define command mapping string -> function.
    commands["help"] = HelpCommand;
    commands["tp"] = TPCommand;
    commands["spawnpoint"] = SpawnpointCommand;
    commands["gravity"] = SetGravityCommand;
    commands["show_chunk_borders"] = SetChunkBorderVisibilityCommand;
    commands["setblock"] = SetBlockCommand;
    commands["getblock"] = GetBlockCommand;

    std::cout << "Constructed Console!\n";
}

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

        // If it's not a command, print it as a message instead.
        if (input[0] != '/')
            std::cout << input << "\n";
        else {
            // Remove '/'
            input.erase(0, 1);

            ExecuteCommand(input);
        }

        Close();
    }

    int key = GetCharPressed();
    if (key != 0) {
        input += static_cast<char>(key);
    }
}

void Console::Render() {
    if (!open) return;

    float hSpacing = 4.f, vSpacing = 16.f;
    float width = 512, height = 32;

    Rectangle area = {hSpacing, static_cast<float>(Settings::windowHeight) - vSpacing - height, width, height};
    DrawRectangle(area.x, area.y, area.width, area.height, {0, 0, 0, 180});

    float tSpacing = 4.f;
    std::string renderedText = input;
    
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    if (ms % 1000 < 500)
        renderedText += '|';

    DrawText(renderedText.c_str(), static_cast<int>(area.x + tSpacing), static_cast<int>(area.y + tSpacing), 24, {255, 255, 255, 240});
}

void Console::ExecuteCommand(const std::string& command) {
    std::stringstream ss(command);
    
    // Object which holds each token.
    std::string token;

    std::vector<std::string> tokens;

    while (ss >> token)
        tokens.push_back(token);

    // Find command with name.
    auto it = commands.find(tokens[0]);

    if (it != commands.end()) {
        it->second(tokens, gameplaySession);
    } else {
        std::cout << "Unknown Command '" << tokens[0] << "'.\n";
    }
}