#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Entity.hpp"
#include <memory>

bool mouseHit(const Vector &mousePos, std::shared_ptr<Entity> entity);

#endif