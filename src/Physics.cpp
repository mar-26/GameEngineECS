#include "../include/Physics.hpp"

bool mouseRectHit(const Vector &mousePos, std::shared_ptr<Entity> entity)
{
    auto boundingBox = entity->getComponent<CBoundingBox>().m_half_size;
    auto position = entity->getComponent<CTransform>().m_position;
    if (mousePos.x > position.x-(boundingBox.x) && mousePos.y > position.y-(boundingBox.y) &&
        mousePos.x < position.x+(boundingBox.x) && mousePos.y < position.y+(boundingBox.y))
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

Vector rectRectCollision(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
{
    Vector position1 = entity1->getComponent<CTransform>().m_position;
    Vector position2 = entity2->getComponent<CTransform>().m_position;
    Vector size1 = entity1->getComponent<CBoundingBox>().m_half_size;
    Vector size2 = entity2->getComponent<CBoundingBox>().m_half_size;

    float dx = abs(position1.x - position2.x);
    float dy = abs(position1.y - position2.y);
    float ox = size1.x + size2.x - dx;
    float oy = size1.y + size2.y - dy;

    return Vector(ox, oy);
}

Vector prevRectRectCollision(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2)
{
    Vector position1 = entity1->getComponent<CTransform>().m_prev_position;
    Vector position2 = entity2->getComponent<CTransform>().m_prev_position;
    Vector size1 = entity1->getComponent<CBoundingBox>().m_half_size;
    Vector size2 = entity2->getComponent<CBoundingBox>().m_half_size;

    float dx = abs(position1.x - position2.x);
    float dy = abs(position1.y - position2.y);
    float ox = size1.x + size2.x - dx;
    float oy = size1.y + size2.y - dy;

    return Vector(ox, oy);
}

float lerp(float v0, float v1, float t)
{
    return (1-t) * v0 + t * v1;
}