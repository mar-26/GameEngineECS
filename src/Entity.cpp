#include "../include/Entity.hpp"
#include <memory>

void Entity::destroy()
{
    m_is_alive = false;
}

const size_t Entity::id() const
{
    return m_id;
}

const bool Entity::isAlive() const
{
    return m_is_alive;
}

const std::string& Entity::tag() const
{
    return m_tag;
}