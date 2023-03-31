#ifndef SCENE_HPP
#define SCENE_HPP

#include "Assets.hpp"
#include "Action.hpp"
#include "EntityManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
    public:
        Scene() {}
        Scene(GameEngine* gameEngine) : m_game(gameEngine) {};

        virtual void update() = 0;
        virtual void sRender() = 0;
        virtual void sDoAction(const Action& action) = 0;
        virtual void loadAssets() = 0;

        bool hasEnded() const;
        void registerAction(const int key, const std::string& actionName);
        const ActionMap& getActionMap() const;

    protected:
        GameEngine*   m_game = nullptr;
        EntityManager m_entity_manager;
        ActionMap     m_action_map;
        Assets        m_scene_assets;
        bool          m_paused = false;
        bool          m_has_ended = false;

        virtual void onEnd() = 0;
        void setPaused(bool paused);
};

#endif