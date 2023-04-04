#include "../include/ScenePlatformer.hpp"
#include "../include/GameEngine.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

ScenePlatformer::ScenePlatformer(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
    m_debug_value = 1;
}

void ScenePlatformer::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));

}

void ScenePlatformer::loadAssets()
{
    m_scene_assets.addTexture("background", "assets/textures/platformer_background.png", true, true);
}

void ScenePlatformer::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
        sCollisions();
    }

#ifdef DEBUG
    ImGui::SFML::Update(m_game->window(), m_delta_clock.restart());
    ImGui::Begin("Debug");
    ImGui::Checkbox("Debug", &m_debug);
    ImGui::SliderFloat("debug value", &m_debug_value, 0.1, 1);
    ImGui::End();
#endif

}

void ScenePlatformer::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { m_game->quit(); }
    }
    else if (action.type() == "END")
    {
    }
}

void ScenePlatformer::sMovement()
{
}

void ScenePlatformer::sCollisions()
{
}

void ScenePlatformer::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear();
    }
    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    m_game->window().draw(m_background);

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void ScenePlatformer::onEnd()
{
    m_game->quit();
}

ScenePlatformer::~ScenePlatformer()
{
}