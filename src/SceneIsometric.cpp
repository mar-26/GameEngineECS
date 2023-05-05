#include "../include/SceneIsometric.hpp"
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

SceneIsometric::SceneIsometric(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Isometric\n";
    init();
    m_debug_value = 1;
}

void SceneIsometric::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Space, "JUMP");
    registerAction(sf::Keyboard::A, "MOVE_LEFT");
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");

    loadAssets();

}

void SceneIsometric::loadAssets()
{
}

void SceneIsometric::update()
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
    ImGui::SliderFloat("scale", &m_scale, 0, 2);
    ImGui::SliderFloat("skew", &m_skew, 0, 2*M_PI);
    ImGui::SliderFloat("angle", &m_angle, 0, 2*M_PI);
    ImGui::End();
#endif

}

void SceneIsometric::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { onEnd(); }
    }
    else if (action.type() == "END")
    {
    }
}

void SceneIsometric::sMovement()
{

}

void SceneIsometric::sCollisions()
{
}

void SceneIsometric::sAnimation()
{
}

void randomColor()
{
    float r = rand()%10*0.1;
    float g = rand()%10*0.1;
    float b = rand()%10*0.1;
    glColor3f(r, g, b);
}

void SceneIsometric::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear(sf::Color::White);

        float size = 10;

    }
    else
    {
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void SceneIsometric::onEnd()
{
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

SceneIsometric::~SceneIsometric()
{
}