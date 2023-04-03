#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Entity.hpp"
#include <memory>

bool mouseRectHit(const Vector &mousePos, std::shared_ptr<Entity> entity);
bool circleCircleHit(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);

#endif