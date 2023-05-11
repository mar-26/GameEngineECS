#include "../include/SceneStaticAnalysis.hpp"
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

SceneStaticAnalysis::SceneStaticAnalysis(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Isometric\n";
    init();
    m_debug_value = 1;
    m_view.setSize(m_game->width(), m_game->height());
    m_view.setCenter(m_game->width()/2, m_game->height()/2);

    m_hud_view.setSize(m_game->width(), 200);
    m_hud_view.setCenter(m_game->width()/2, m_game->height()/2);
    m_hud_view.setViewport(sf::FloatRect(0, m_game->height()-200, 1, 1));
}

void SceneStaticAnalysis::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");

    loadAssets();

}

void SceneStaticAnalysis::loadAssets()
{
}

void SceneStaticAnalysis::update()
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
    ImGui::Checkbox("Debug", &m_debug);
    ImGui::SliderFloat("hud height", &m_debug_value, 0, m_game->height());
    ImGui::End();
#endif

    m_hud_view.setSize(m_game->width(), m_debug_value);
    m_hud_view.setCenter(m_game->width()/2, m_game->height()/2);
    m_hud_view.setViewport(sf::FloatRect(0, m_game->height()-m_debug_value, 1, 1));

}

void SceneStaticAnalysis::sDoAction(const Action &action)
{

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

            m_mousePos = Vector(convertedMousePos.x, convertedMousePos.y);
            mouseLeftPressed = true;
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
            mouseLeftPressed = false;
            m_mousePos = Vector();
            m_mouseMovePos = Vector();
            
        }
    }
}

void SceneStaticAnalysis::sMovement()
{
    if (mouseLeftPressed)
    {
        if (m_mouseMovePos == Vector())
        {
            m_mouseMovePos = m_mousePos;
        }
        Vector newPos = (m_mouseMovePos - m_mousePos).normalize();
        m_view.move(-newPos.x, -newPos.y);

    }
}

void SceneStaticAnalysis::sCollisions()
{
}

void SceneStaticAnalysis::sAnimation()
{
}

void SceneStaticAnalysis::sRender()
{
    int subSize = 16;
    if (!m_paused)
    {
        m_game->window().setView(m_view);
        m_game->window().clear(sf::Color::White);

        if (mouseLeftPressed)
        {
            sf::Vertex* line = debugLine(m_mousePos, m_mouseMovePos, sf::Color::Red);
            m_game->window().draw(line, 2, sf::Lines);
        }

        for (int i = 0; i < m_game->height()/subSize; i++)
        {
            for (int j = 0; j < m_game->width()/subSize; j++)
            {
                sf::Vertex* horizontalLine = debugLine(Vector(j*subSize, i*subSize+subSize), Vector(j*subSize+subSize, i*subSize+subSize), sf::Color::Black);
                sf::Vertex* verticalLine = debugLine(Vector(j*subSize, i*subSize), Vector(j*subSize, i*subSize+subSize), sf::Color::Black);
                m_game->window().draw(horizontalLine, 2, sf::Lines);
                m_game->window().draw(verticalLine, 2, sf::Lines);
            }
        }

        m_game->window().setView(m_hud_view);
        sf::RectangleShape hud(sf::Vector2f(m_game->width(), m_debug_value));
        hud.setFillColor(sf::Color::Red);
        m_game->window().draw(hud);
    }
    else
    {
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void SceneStaticAnalysis::onEnd()
{
    m_view.reset(sf::FloatRect(0, 0, 1280.f, 768.f));
    m_game->window().setView(m_view);
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

Vector SceneStaticAnalysis::windowToWorld(const Vector& window) const
{
    sf::View view = m_game->window().getView();

    float wx = view.getCenter().x - (m_game->window().getSize().x / 2);
    float wy = view.getCenter().y - (m_game->window().getSize().y / 2);

    return Vector(window.x + wx, window.y + wy);
}

SceneStaticAnalysis::~SceneStaticAnalysis()
{
}