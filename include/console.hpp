#pragma once

#include <string>
#include <iostream>

class Console {
public:
    bool open = false;
    std::string input = "";

    Console() { std::cout << "Constructed Console!\n"; }
    ~Console() { std::cout << "Deconstructed Console!\n"; }

    void Update();

private:
    void Open() { open = true; input.clear(); std::cout << "Opened Console!\n"; }
    void Close() { open = false; input.clear(); std::cout << "Closed Console!\n"; }

    void ExecuteCommand(const std::string& command);
};