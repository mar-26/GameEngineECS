#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "Entity.hpp"

#include <memory>
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager
{
    public:
        EntityManager();

        void update();
        std::shared_ptr<Entity> addEntity(const std::string& tag);
        const EntityVector& getEntities();
        const EntityVector& getEntities(const std::string& tag);
    
    private:
        void removeDeadEntities(EntityVector& entityVec);

    private:
        EntityVector m_entities;
        EntityVector m_entities_to_add;
        EntityMap    m_entity_map;
        size_t       m_entity_count = 0;
};

#endif