#include "../include/LevelMaker.hpp"
#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>

LevelMaker::LevelMaker(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Isometric\n";
    init();
    m_debug_value = 1;
    m_view.setSize(m_game->width(), m_game->height());
    m_view.setCenter(m_game->width()/2, m_game->height()/2);

}

void LevelMaker::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");

    loadAssets();
    m_background.setTexture(m_scene_assets.getTexture("platformer_background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));
}

void LevelMaker::loadAssets()
{
    m_scene_assets.addTexture("platformer_background", "assets/textures/platformer_background.png", true, true);
    m_scene_assets.addTexture("white_floor_tile", "assets/textures/scifi_platform_tiles.png", sf::IntRect(0, 192, 32, 32), true, false);
    m_scene_assets.addTexture("caution_topleft_tile", "assets/textures/scifi_platform_tiles.png", sf::IntRect(160, 128, 32, 32), true, false);
    m_scene_assets.addTexture("xbox_tile", "assets/textures/scifi_platform_tiles.png", sf::IntRect(96, 160, 32, 32), true, false);
}

void LevelMaker::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
        sCollisions();
        sAnimation();
    }

#ifdef DEBUG
    ImGui::SFML::Update(m_game->window(), m_delta_clock.restart());
    ImGui::Begin("Debug");
    ImGui::Checkbox("Collision", &m_collision);
    ImGui::Combo("tiles", &m_current_tile, tileTypes, IM_ARRAYSIZE(tileTypes));
    ImGui::End();
#endif

}

void LevelMaker::sDoAction(const Action &action)
{
//    if (ImGui::IsMouseHoveringAnyWindow())
//    {
//        printf("here\n");
//    }
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { onEnd(); }
        if (action.name() == "MOUSE_WHEEL")
        {
            // limit zoom out
            float xSize = m_view.getSize().x;
            float ySize = m_view.getSize().y;
            m_zoom = (action.delta() > 0) ? 1.01 : 0.99;
            if (xSize*m_zoom < m_game->width() && ySize*m_zoom < m_game->height())
            {
                m_view.zoom(m_zoom);
            }
            else
            {
                m_view.reset(sf::FloatRect(0, 0, m_game->width(), m_game->height()));
            }
        }
        if (action.name() == "MOUSE_LEFT")
        {
            // for keeping screen coordinates and world coordinates the same
            sf::Vector2i mousePos(action.pos().x, action.pos().y);
            sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);

            Vector tileCoords = tileCenter(convertedMousePos);


            const char* tType = tileTypes[m_current_tile];
            sf::Sprite sprite;
            if (tType == "white_floor")
            {
                sprite.setTexture(m_scene_assets.getTexture("white_floor_tile"));
            }
            else if (tType == "caution_topleft")
            {
                sprite.setTexture(m_scene_assets.getTexture("caution_topleft_tile"));
            }
            else if (tType == "xbox")
            {
                sprite.setTexture(m_scene_assets.getTexture("xbox_tile"));
            }

            sprite.setOrigin(m_tile_size/2, m_tile_size/2);
            sprite.setPosition(tileCoords.x, tileCoords.y);
            tiles.push_back(sprite);

//
//            m_mousePos = Vector(convertedMousePos.x, convertedMousePos.y);
//            mouseLeftPressed = true;
        }
        if (action.name()  == "MOUSE_MOVE")
        {
            sf::Vector2i mousePos(action.pos().x, action.pos().y);
            sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);
            m_mouseMovePos = Vector(convertedMousePos.x, convertedMousePos.y);
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOUSE_LEFT")
        {
//            mouseLeftPressed = false;
//            m_mousePos = Vector();
//            m_mouseMovePos = Vector();
//            
        }
    }
}

void LevelMaker::sMovement()
{
//    if (mouseLeftPressed)
//    {
//        if (m_mouseMovePos == Vector())
//        {
//            m_mouseMovePos = m_mousePos;
//        }
//        Vector newPos = (m_mouseMovePos - m_mousePos).normalize();
//        m_view.move(-newPos.x, -newPos.y);
//
//    }
}

void LevelMaker::sCollisions()
{
}

void LevelMaker::sAnimation()
{
}

void LevelMaker::sRender()
{
    if (!m_paused)
    {
        m_game->window().setView(m_view);
        m_game->window().clear(sf::Color::White);
        m_game->window().draw(m_background);

        if (mouseLeftPressed)
        {
            sf::Vertex* line = debugLine(m_mousePos, m_mouseMovePos, sf::Color::Red);
            m_game->window().draw(line, 2, sf::Lines);
        }

        for (int i = 0; i < tiles.size(); i++)
        {
            m_game->window().draw(tiles[i]);
        }

        for (int i = 0; i < m_game->height()/m_tile_size; i++)
        {
            for (int j = 0; j < m_game->width()/m_tile_size; j++)
            {
                sf::Vertex* horizontalLine = debugLine(Vector(j*m_tile_size, i*m_tile_size+m_tile_size), Vector(j*m_tile_size+m_tile_size, i*m_tile_size+m_tile_size), sf::Color::Black);
                sf::Vertex* verticalLine = debugLine(Vector(j*m_tile_size, i*m_tile_size), Vector(j*m_tile_size, i*m_tile_size+m_tile_size), sf::Color::Black);
                m_game->window().draw(horizontalLine, 2, sf::Lines);
                m_game->window().draw(verticalLine, 2, sf::Lines);
            }
        }
    }
    else
    {
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void LevelMaker::onEnd()
{
    m_view.reset(sf::FloatRect(0, 0, 1280.f, 768.f));
    m_game->window().setView(m_view);
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

Vector LevelMaker::windowToWorld(const Vector& window) const
{
    sf::View view = m_game->window().getView();

    float wx = view.getCenter().x - (m_game->window().getSize().x / 2);
    float wy = view.getCenter().y - (m_game->window().getSize().y / 2);

    return Vector(window.x + wx, window.y + wy);
}

Vector LevelMaker::tileCenter(const sf::Vector2f& mouseCoords) const
{
    Vector tile = Vector((int)(mouseCoords.x/m_tile_size), (int)(mouseCoords.y/m_tile_size));
    int halfTile = m_tile_size/2;
    return Vector(tile.x*m_tile_size + halfTile, tile.y*m_tile_size + halfTile);
}

LevelMaker::~LevelMaker()
{
}