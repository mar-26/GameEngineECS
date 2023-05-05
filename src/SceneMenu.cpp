#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/SceneStarship.hpp"
#include "../include/ScenePlatformer.hpp"
#include "../include/SceneIsometric.hpp"
#include "../include/SceneStaticAnalysis.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Window/Keyboard.hpp>

SceneMenu::SceneMenu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Menu\n";
    init();
}

void SceneMenu::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::D, "Debug");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("menu_background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));

    float buttonWidth = 200;
    float buttonHeight = 50;
    Vector starshipButtonPosition = Vector((m_game->width()/2.f), (m_game->height()/2.f)-100);
    Vector platformerButtonPosition = Vector((m_game->width()/2.f), (m_game->height()/2.f));
    Vector isometricButtonPosition = Vector((m_game->width()/2.f), (m_game->height()/2.f)+100);
    Vector staticAnalysisButtonPosition = Vector((m_game->width()/2.f), (m_game->height()/2.f)+200);

    createButton("play_starship_button", starshipButtonPosition, "play_starship_button_texture", buttonWidth, buttonHeight);
    createButton("play_platformer_button", platformerButtonPosition, "play_platformer_button_texture", buttonWidth, buttonHeight);
    createButton("play_isometric_button", isometricButtonPosition, "play_isometric_button_texture", buttonWidth, buttonHeight);
    createButton("static_analysis_button", staticAnalysisButtonPosition, "static_analysis_button_texture", buttonWidth, buttonHeight);

    m_menu_music = m_scene_assets.getMusic("menu_music");
    m_menu_music->play();
}

void SceneMenu::loadAssets()
{
    m_scene_assets.addTexture("menu_background", "assets/textures/rock.png", true, true);
    m_scene_assets.addTexture("play_starship_button_texture", "assets/textures/buttons/play_starship_button.png", true, false);
    m_scene_assets.addTexture("play_platformer_button_texture", "assets/textures/buttons/play_platformer_button.png", true, false);
    m_scene_assets.addTexture("play_isometric_button_texture", "assets/textures/buttons/play_isometric_button.png", true, false);
    m_scene_assets.addTexture("static_analysis_button_texture", "assets/textures/buttons/static_analysis_button.png", true, false);
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
            auto box = e->getComponent<CBoundingBox>();
            sf::Vertex* outline = debugRectangle(Vector(transform.m_position.x, transform.m_position.y), box.m_half_size, sf::Color::Red);
            sf::Vertex* leftLine = debugLine(Vector(transform.m_position.x, 0), Vector(transform.m_position.x, m_game->height()), sf::Color::Green);
            sf::Vertex* topLine = debugLine(Vector(0, transform.m_position.y), Vector(m_game->width(), transform.m_position.y), sf::Color::Green);
            m_game->window().draw(leftLine, 2, sf::Lines);
            m_game->window().draw(topLine, 2, sf::Lines);
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

        if (action.name() == "Debug")
        {
            m_debug = !m_debug;
        }
        if (action.name() == "MOUSE_LEFT")
        {
            for (auto button : m_entity_manager.getEntities())
            {
                if (button->tag() == "play_starship_button" && mouseRectHit(action.pos(), button))
                {
                    m_menu_music->stop();
                    m_game->changeScene("STARSHIP", std::make_shared<SceneStarship>(m_game));
                }
                if (button->tag() == "play_platformer_button" && mouseRectHit(action.pos(), button))
                {
                    m_menu_music->stop();
                    m_game->changeScene("PLATFORMER", std::make_shared<ScenePlatformer>(m_game));
                }
                if (button->tag() == "play_isometric_button" && mouseRectHit(action.pos(), button))
                {
                    m_menu_music->stop();
                    m_game->changeScene("ISOMETRIC", std::make_shared<SceneIsometric>(m_game));
                }
                if (button->tag() == "static_analysis_button" && mouseRectHit(action.pos(), button))
                {
                    m_menu_music->stop();
                    m_game->changeScene("STATICANALYSIS", std::make_shared<SceneStaticAnalysis>(m_game));
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

void SceneMenu::createButton(const std::string& name, const Vector& position, const std::string& textureName, float width, float height)
{
    auto button = m_entity_manager.addEntity(name);
    button->addComponent<CTransform>(position);
    button->addComponent<CSprite>(m_scene_assets.getTexture(textureName));
    button->addComponent<CBoundingBox>(Vector(width, height));
}

SceneMenu::~SceneMenu()
{
    delete m_menu_music;
}