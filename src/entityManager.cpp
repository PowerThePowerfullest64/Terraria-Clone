#include "entityManager.hpp"

#include <iostream>

EntityManager::EntityManager() {
    std::cout << "Constructed EntityManager!\n";
}

EntityManager::~EntityManager() {
    std::cout << "Deconstructed EntityManager!\n";
}