#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/ScenePlay.hpp"
#include "../include/Physics.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
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
    registerAction(sf::Keyboard::D, "Debug");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("menu_background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));

    Vector menuButtonPosition = Vector(m_game->width()/2.f, m_game->height()/2.f);
    float buttonWidth = 200;
    float buttonHeight = 50;
    m_menu_button = m_entity_manager.addEntity("menu_button");
    m_menu_button->addComponent<CTransform>(menuButtonPosition);
    m_menu_button->addComponent<CSprite>(m_scene_assets.getTexture("play_button"));
    m_menu_button->addComponent<CBoundingBox>(sf::FloatRect(menuButtonPosition.x-(buttonWidth/2), menuButtonPosition.y-(buttonHeight/2), buttonWidth, buttonHeight));

    m_menu_music = m_scene_assets.getMusic("menu_music");
    m_menu_music->play();
}

void SceneMenu::loadAssets()
{
    m_scene_assets.addTexture("menu_background", "assets/textures/rock.png", true, true);
    m_scene_assets.addTexture("play_button", "assets/textures/buttons/play_button.png", true, true);
    m_scene_assets.addMusic("menu_music", "assets/sounds/menu_music.ogg");
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

        if (e->hasComponent<CBoundingBox>() && m_debug)
        {
            auto& outline = e->getComponent<CBoundingBox>().m_outline;
            m_game->window().draw(outline, 5, sf::LineStrip);
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
            m_menu_music->stop();
            m_game->changeScene("GAME", std::make_shared<ScenePlay>(m_game));
        }
        if (action.name() == "Debug")
        {
            m_debug = !m_debug;
        }
        if (action.name() == "MOUSE_LEFT")
        {
            for (auto button : m_entity_manager.getEntities("menu_button"))
            {
                if (mouseHit(action.pos(), button))
                {
                    m_menu_music->stop();
                    m_game->changeScene("GAME", std::make_shared<ScenePlay>(m_game));
                }
            }
        }
    }
}

void SceneMenu::sCollisions()
{

}

void SceneMenu::onEnd()
{
    m_game->quit();
}

SceneMenu::~SceneMenu()
{
    delete m_menu_music;
}