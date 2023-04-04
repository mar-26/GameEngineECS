#include "../include/Physics.hpp"

bool mouseRectHit(const Vector &mousePos, std::shared_ptr<Entity> entity)
{
    auto boundingBox = entity->getComponent<CBoundingBox>().m_box;
    if (mousePos.x > boundingBox.left && mousePos.y > boundingBox.top &&
        mousePos.x < boundingBox.left+boundingBox.width && mousePos.y < boundingBox.top+boundingBox.height)
    {
        return true;
    }
    return false;
}

bool circleCircleHit(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
{
    auto position1 = entity1->getComponent<CTransform>().m_position;
    auto position2 = entity2->getComponent<CTransform>().m_position;

    auto circle1 = entity1->getComponent<CBoundingCircle>();
    auto circle2 = entity2->getComponent<CBoundingCircle>();

    Vector distVect = position1 - position2;
    float dist = distVect.magnitude();

    if (dist < circle1.m_radius + circle2.m_radius)
    {
        return true;
    }
    return false;
}

float lerp(float v0, float v1, float t)
{
    return (1-t) * v0 + t * v1;
}