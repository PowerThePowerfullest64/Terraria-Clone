#pragma once

#include <iostream>
#include <string>

#include "gameplaySession.hpp"
#include "entity.hpp"
#include "world.hpp"
#include "settings.hpp"

bool StringToBool(const std::string& s) {
    return s == "1" || s == "true";
}

void HelpCommand(const std::vector<std::string>& args, GameplaySession* gameplaySession) {
    std::cout << "Current available commands are: \n";
    std::cout << " - /tp <target> <x> <y>\n";
    std::cout << " - /spawnpoint <x> <y>\n";
    std::cout << " - /gravity <value>\n";
    std::cout << " - /show_chunk_borders <value>\n";

    std::cout << "@s = self, @e = all entities\n";
    std::cout << "Also use ~ to use default/current position component. :)\n";
}

void TPCommand(const std::vector<std::string>& args, GameplaySession* gameplaySession) {
    std::string target = args[1];
    std::string _x = args[2];
    std::string _y = args[3];

    if (target == "@s") {
        Entity* e = gameplaySession->player;

        int x =  (_x == "~") ? World::ToWorld(e->position).x : std::stoi(_x);
        int y = (_y == "~") ? World::ToWorld(e->position).y : std::stoi(_y);

        e->position = World::FromWorld(x, y);

    } else if (target == "@e") {
        for (Entity* e : gameplaySession->em->GetAllEntities()) {
            int x =  (_x == "~") ? World::ToWorld(e->position).x : std::stoi(_x);
            int y = (_y == "~") ? World::ToWorld(e->position).y : std::stoi(_y);

            e->position = World::FromWorld(x, y);
        }
    }
}

void SpawnpointCommand(const std::vector<std::string>& args, GameplaySession* gameplaySession) {
    std::string _x = args[1];
    std::string _y = args[2];

    int x =  (_x == "~") ? gameplaySession->spawnPoint.x : std::stoi(_x);
    int y = (_y == "~") ? gameplaySession->spawnPoint.y : std::stoi(_y);

    gameplaySession->spawnPoint.x = x;
    gameplaySession->spawnPoint.y = y;
}

void SetGravityCommand(const std::vector<std::string>& args, GameplaySession* gameplaySession) {
    int value = std::stoi(args[1]);

    Settings::gravity = value;
}

void SetChunkBorderVisibilityCommand(const std::vector<std::string>& args, GameplaySession* gameplaySession) {
    bool value = StringToBool(args[1]);

    Settings::showChunkBorders = value;
}