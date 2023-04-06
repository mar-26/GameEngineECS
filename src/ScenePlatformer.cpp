#include "../include/ScenePlatformer.hpp"
#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Graphics/Rect.hpp>
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
    registerAction(sf::Keyboard::A, "MOVE_LEFT");
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");

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
        if (action.name() == "QUIT") { onEnd(); }
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

        m_game->window().draw(m_background);

        if (m_debug)
        {

            for (int i = 0; i < m_game->height(); i+=32)
            {
                sf::Vertex* horizontalLine = debugLine(Vector(0, i), Vector(m_game->width(), i), sf::Color::White);
                m_game->window().draw(horizontalLine, 2, sf::Lines);
            }
            for (int i = 0; i < m_game->width(); i+=32)
            {
                sf::Vertex* verticalLine = debugLine(Vector(i, 0), Vector(i, m_game->height()), sf::Color::White);
                m_game->window().draw(verticalLine, 2, sf::Lines);
            }
        }

        for (auto e : m_entity_manager.getEntities())
        {
            auto transform = e->getComponent<CTransform>();
            auto& sprite = e->getComponent<CSprite>().m_sprite;
            sprite.setPosition(sf::Vector2f(transform.m_position.x, transform.m_position.y));
            m_game->window().draw(sprite);

        }
    }
    else
    {
        m_game->window().clear();
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void ScenePlatformer::onEnd()
{
    m_game->changeScene("GAME", std::make_shared<SceneMenu>(m_game));
}

ScenePlatformer::~ScenePlatformer()
{
}