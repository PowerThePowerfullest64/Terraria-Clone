#include "entityManager.hpp"

#include <memory>
#include <iostream>

std::vector<std::unique_ptr<Entity>> EntityManager::entities;