#include "../include/EntityManager.hpp"

#include <assert.h>
#include <memory>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    for (auto e : m_entities_to_add)
    {
        if (e->tag() == "player")
        {
            continue;
        }
        m_entities.push_back(e);
        m_entity_map[e->tag()].push_back(e);
    }

    m_entities_to_add.clear();

    removeDeadEntities(m_entities);

    for (auto& kv : m_entity_map)
    {
        removeDeadEntities(kv.second);
    }
}

void EntityManager::removeDeadEntities(EntityVector& entityVec)
{
    entityVec.erase(std::remove_if(entityVec.begin(), entityVec.end(),
            [](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }), entityVec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(m_entity_count, tag));
    m_entities_to_add.push_back(e);
    return e;
}

const EntityVector& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag)
{
    return m_entity_map[tag];
}