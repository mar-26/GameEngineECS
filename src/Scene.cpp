#include "../include/Scene.hpp"
#include <SFML/Graphics/Vertex.hpp>


bool Scene::hasEnded() const
{
    return m_has_ended;
}

void Scene::registerAction(const int key, const std::string& actionName)
{
    m_action_map[key] = actionName;
}

const ActionMap& Scene::getActionMap() const
{
    return m_action_map;
}

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}