#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Component.hpp"

#include <string>
#include <tuple>

typedef std::tuple<
    CTransform,
    CSprite,
    CAnimation,
    CBoundingBox,
    CBoundingCircle,
    CSoundEffect,
    CInput
> ComponentTuple;

class EntityManager;

class Entity
{
    public:
        void destroy();
        const size_t id() const;
        const bool isAlive() const;
        const std::string& tag() const;

        template<typename T>
        bool hasComponent() const
        {
            return getComponent<T>().has;
        }

        template<typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            auto& component = getComponent<T>();
            component = T(std::forward<TArgs>(mArgs)...);
            component.has = true;
            return component;
        }

        template<typename T>
        T& getComponent()
        {
            return std::get<T>(m_components);
        }

        template<typename T>
        const T& getComponent() const
        {
            return std::get<T>(m_components);
        }

        template<typename T>
        void removeComponent()
        {
            getComponent<T>() = T();
        }

    friend class EntityManager;

    private:
        Entity(const size_t& id, const std::string& tag)
            : m_id(id), m_tag(tag) {}

        ComponentTuple m_components;
        std::string m_tag = "default";
        bool m_is_alive = true;
        size_t m_id = 0;
};

#endif