#include "../include/Physics.hpp"

bool mouseHit(const Vector &mousePos, std::shared_ptr<Entity> entity)
{
    auto boundingBox = entity->getComponent<CBoundingBox>().m_box;
    if (mousePos.x > boundingBox.left && mousePos.y > boundingBox.top &&
        mousePos.x < boundingBox.left+boundingBox.width && mousePos.y < boundingBox.top+boundingBox.height)
    {
        return true;
    }
    return false;
}