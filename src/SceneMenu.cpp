#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/ScenePlay.hpp"

#include <SFML/Window/Keyboard.hpp>

SceneMenu::SceneMenu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void SceneMenu::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Enter, "PLAY");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("menu_background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));
}

void SceneMenu::loadAssets()
{
    m_scene_assets.addTexture("menu_background", "assets/textures/rock.png", true, true);
}

void SceneMenu::update()
{
    m_entity_manager.update();
}

void SceneMenu::sRender()
{
    m_game->window().clear(sf::Color(255, 0, 0));
    m_game->window().draw(m_background);

    for (auto e : m_entity_manager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();

        if (e->hasComponent<CSprite>())
        {
            auto& sprite = e->getComponent<CSprite>().m_sprite;
            sprite.setPosition(transform.m_position.x, transform.m_position.y);
            m_game->window().draw(sprite);
        }
    }
}

void SceneMenu::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT")
        {
            onEnd();
        }

        if (action.name() == "PLAY")
        {
            m_game->changeScene("GAME", std::make_shared<ScenePlay>(m_game));
        }
    }
}

void SceneMenu::onEnd()
{
    m_game->quit();
}