#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>

class GameplaySession;

class Console {
public:
    bool open = false;
    std::string input = "";
    GameplaySession* gameplaySession;

    Console(GameplaySession* gameplaySession);
    ~Console() { std::cout << "Deconstructed Console!\n"; }

    void Update();
    void Render();

private:
    std::unordered_map<std::string,
        std::function<void(const std::vector<std::string>&, GameplaySession*)>> commands;

    void Open() { open = true; input.clear(); }
    void Close() { open = false; input.clear(); }

    void ExecuteCommand(const std::string& command);
};