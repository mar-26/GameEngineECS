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
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <sstream>

LevelMaker::LevelMaker(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Level Maker\n";
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

    createButton("save", Vector(m_game->width()/2.f, (m_game->height()/2.f)), "save_button_texture", 200, 100);
    createButton("quit", Vector(m_game->width()/2.f, (m_game->height()/2.f)+100), "quit_button_texture", 200, 100);
}

void LevelMaker::loadAssets()
{
    m_scene_assets.addTexture("platformer_background", "assets/textures/platformer_background.png", true, true);

    m_scene_assets.addTexture("save_button_texture", "assets/textures/buttons/save_button.png", true, false);
    m_scene_assets.addTexture("quit_button_texture", "assets/textures/buttons/quit_button.png", true, false);

    m_scene_assets.addTexture("white_floor", "assets/textures/scifi_platform_tiles.png", sf::IntRect(0, 192, 32, 32), true, false);
    m_scene_assets.addTexture("caution_top_left", "assets/textures/scifi_platform_tiles.png", sf::IntRect(160, 128, 32, 32), true, false);
    m_scene_assets.addTexture("caution_top_right", "assets/textures/scifi_platform_tiles.png", sf::IntRect(192, 128, 32, 32), true, false);
    m_scene_assets.addTexture("caution_bottom_left", "assets/textures/scifi_platform_tiles.png", sf::IntRect(160, 160, 32, 32), true, false);
    m_scene_assets.addTexture("caution_bottom_right", "assets/textures/scifi_platform_tiles.png", sf::IntRect(192, 160, 32, 32), true, false);
    m_scene_assets.addTexture("xbox", "assets/textures/scifi_platform_tiles.png", sf::IntRect(96, 160, 32, 32), true, false);
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
    ImGui::Checkbox("Debug Render", &m_debug_render);
    ImGui::Checkbox("Collision", &m_collision);
    ImGui::Combo("tiles", &m_current_tile, tileTypes, IM_ARRAYSIZE(tileTypes));
    ImGui::End();
#endif

}

void LevelMaker::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { m_paused = !m_paused; }

        // zoom in and out
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

        // place tiles
        if (action.name() == "MOUSE_LEFT")
        {
            if (!m_paused)
            {
                // for keeping screen coordinates and world coordinates the same
                sf::Vector2i mousePos(action.pos().x, action.pos().y);
                sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);

                Vector tileCoords = tileCenter(convertedMousePos);
                bool clearSpot = true;
                for (auto tile : m_entity_manager.getEntities("tile"))
                {
                    if (mouseRectHit(tileCoords, tile) || tile->getComponent<CTransform>().m_position == tileCoords)
                    {
                        clearSpot = false;
                    }
                }
                if (clearSpot)
                {
                    createTile(tileTypes[m_current_tile], tileCoords, m_collision);
                }
            }
            else
            {
                for (auto button : m_entity_manager.getEntities("button"))
                {
                    if (button->getComponent<CAux>().m_aux == "save" && mouseRectHit(action.pos(), button))
                    {
                        if(saveLevel())
                        {
                            onEnd();
                        }
                    }
                    if (button->getComponent<CAux>().m_aux == "quit" && mouseRectHit(action.pos(), button))
                    {
                        onEnd();
                    }
                }
            }
        }

        // eraser
        if (action.name() == "MOUSE_RIGHT")
        {
                sf::Vector2i mousePos(action.pos().x, action.pos().y);
                sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);

                Vector tileCoords = tileCenter(convertedMousePos);
                bool clearSpot = true;
                for (auto tile : m_entity_manager.getEntities("tile"))
                {
                    if (mouseRectHit(tileCoords, tile) || tile->getComponent<CTransform>().m_position == tileCoords)
                    {
                        tile->destroy();
                    }
                }
        }

        // move around
        if (action.name() == "MOUSE_MIDDLE")
        {
            // for keeping screen coordinates and world coordinates the same
            sf::Vector2i mousePos(action.pos().x, action.pos().y);
            sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);
            m_mousePos = Vector(convertedMousePos.x, convertedMousePos.y);
            mouseMiddlePressed = true;
        }
        if (action.name() == "MOUSE_MOVE")
        {
            sf::Vector2i mousePos(action.pos().x, action.pos().y);
            sf::Vector2f convertedMousePos = m_game->window().mapPixelToCoords(mousePos, m_view);
            m_mouseMovePos = Vector(convertedMousePos.x, convertedMousePos.y);
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOUSE_MIDDLE")
        {
            mouseMiddlePressed = false;
            m_mousePos = Vector();
            m_mouseMovePos = Vector();
            
        }
    }
}

void LevelMaker::sMovement()
{
    if (mouseMiddlePressed)
    {
        if (m_mouseMovePos == Vector())
        {
            m_mouseMovePos = m_mousePos;
        }
        Vector newPos = (m_mouseMovePos - m_mousePos).normalize();
        m_view.move(-newPos.x, 0);

    }
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

        if (mouseMiddlePressed)
        {
            sf::Vertex* line = debugLine(m_mousePos, m_mouseMovePos, sf::Color::Red);
            m_game->window().draw(line, 2, sf::Lines);
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

        for (auto e : m_entity_manager.getEntities("tile"))
        {
            auto transform = e->getComponent<CTransform>();

            if (e->hasComponent<CSprite>())
            {
                auto& sprite = e->getComponent<CSprite>().m_sprite;
                sprite.setPosition(sf::Vector2f(transform.m_position.x, transform.m_position.y));
                m_game->window().draw(sprite);
            }

            if (m_debug_render && e->hasComponent<CBoundingBox>())
            {
                auto box = e->getComponent<CBoundingBox>();
                sf::Vertex* outline = debugRectangle(transform.m_position, box.m_half_size, sf::Color::Red);
                sf::Vertex* centerLineX = debugLine(Vector(transform.m_position.x-box.m_half_size.x, transform.m_position.y), Vector(transform.m_position.x+box.m_half_size.x, transform.m_position.y), sf::Color::Green);
                sf::Vertex* centerLineY = debugLine(Vector(transform.m_position.x, transform.m_position.y-box.m_half_size.y), Vector(transform.m_position.x, transform.m_position.y+box.m_half_size.y), sf::Color::Green);
                m_game->window().draw(centerLineX, 2, sf::Lines);
                m_game->window().draw(centerLineY, 2, sf::Lines);
                m_game->window().draw(outline, 5, sf::LineStrip);
            }

        }
    }
    else
    {
        m_game->window().clear(sf::Color::White);
        m_game->window().draw(m_background);

        for (auto button : m_entity_manager.getEntities("button"))
        {
            auto transform = button->getComponent<CTransform>();

            if (button->hasComponent<CSprite>())
            {
                auto& sprite = button->getComponent<CSprite>().m_sprite;
                sprite.setPosition(sf::Vector2f(transform.m_position.x, transform.m_position.y));
                m_game->window().draw(sprite);
            }

            if (m_debug_render && button->hasComponent<CBoundingBox>())
            {
                auto box = button->getComponent<CBoundingBox>();
                sf::Vertex* outline = debugRectangle(transform.m_position, box.m_half_size, sf::Color::Red);
                sf::Vertex* centerLineX = debugLine(Vector(transform.m_position.x-box.m_half_size.x, transform.m_position.y), Vector(transform.m_position.x+box.m_half_size.x, transform.m_position.y), sf::Color::Green);
                sf::Vertex* centerLineY = debugLine(Vector(transform.m_position.x, transform.m_position.y-box.m_half_size.y), Vector(transform.m_position.x, transform.m_position.y+box.m_half_size.y), sf::Color::Green);
                m_game->window().draw(centerLineX, 2, sf::Lines);
                m_game->window().draw(centerLineY, 2, sf::Lines);
                m_game->window().draw(outline, 5, sf::LineStrip);
            }

        }
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

void LevelMaker::createTile(const std::string& textureName, const Vector& position, bool collision)
{
    auto tile = m_entity_manager.addEntity("tile");
    tile->addComponent<CSprite>(m_scene_assets.getTexture(textureName));
    tile->addComponent<CTransform>(position);
    tile->addComponent<CAux>(textureName);
    if (collision)
    {
        sf::IntRect spriteRect = tile->getComponent<CSprite>().m_sprite.getTextureRect();
        tile->addComponent<CBoundingBox>(spriteRect);
    }
}

void LevelMaker::createButton(const std::string& name, const Vector& position, const std::string& textureName, float width, float height)
{
    auto button = m_entity_manager.addEntity("button");
    button->addComponent<CTransform>(position);
    button->addComponent<CSprite>(m_scene_assets.getTexture(textureName));
    sf::IntRect spriteRect = button->getComponent<CSprite>().m_sprite.getTextureRect();
    button->addComponent<CBoundingBox>(spriteRect);
    button->addComponent<CAux>(name);
}

std::string GenerateUniqueFileName(const std::string& directory)
{
    // Get the current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    // Convert the timestamp to a string
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y%m%d%H%M%S");
    
    // Generate a random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    int randomNum = dis(gen);
    
    // Combine the timestamp and random number to create the unique filename
    std::string uniqueFileName = directory + "/level_" + ss.str() + "_" + std::to_string(randomNum) + ".txt";
    
    return uniqueFileName;
}

bool LevelMaker::saveLevel()
{
    std::string fileName = GenerateUniqueFileName("../levels");
    std::ofstream outputFile(fileName);

    if (!outputFile)
    {
        std::cout << "Error opening " << fileName << "\n";
        return false;
    }
    for (auto tile : m_entity_manager.getEntities("tile"))
    {
        Vector tileTransform = tile->getComponent<CTransform>().m_position;
        outputFile << tile->getComponent<CAux>().m_aux << " " << tileTransform.x << " " << tileTransform.y << " ";
        if (tile->hasComponent<CBoundingBox>())
        {
            outputFile << "C";
        }
        else
        {
            outputFile << "N";
        }
        outputFile << "\n";
    }

    outputFile.close();
    return true;
}

LevelMaker::~LevelMaker()
{
}