#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Entity.hpp"
#include <memory>

bool mouseRectHit(const Vector &mousePos, std::shared_ptr<Entity> entity);
bool circleCircleHit(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);
Vector rectRectCollision(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);
Vector prevRectRectCollision(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2);
float lerp(float v0, float v1, float t);


#endif