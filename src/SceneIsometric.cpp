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

void drawRect(float x, float y, float size, sf::Vector3f color)
{
    glBegin(GL_TRIANGLE_STRIP);
        glColor3f(color.x, color.y, color.z);
        glVertex2f(x - size, y + size);
        glVertex2f(x - size, y - size);
        glVertex2f(x + size, y + size);
        glVertex2f(x + size, y - size);
        glVertex2f(x - size, y - size);
    glEnd();
}

void drawRect(Vector pos, float size, sf::Vector3f color)
{
    glBegin(GL_TRIANGLE_STRIP);
        glColor3f(color.x, color.y, color.z);
        glVertex2f(pos.x - size, pos.y + size);
        glVertex2f(pos.x - size, pos.y - size);
        glVertex2f(pos.x + size, pos.y + size);
        glVertex2f(pos.x + size, pos.y - size);
        glVertex2f(pos.x - size, pos.y - size);
    glEnd();
}

Vector SceneIsometric::matrixMult(Vector pos)
{
    return Vector(pos.x*cos(m_angle) + pos.y*(cos(m_angle)*(m_scale*tan(m_skew))+(-sin(m_angle)*m_scale)), pos.x*sin(m_angle) + pos.y*(sin(m_angle)*(m_scale*tan(m_skew))+(cos(m_angle)*m_scale)));
}

void SceneIsometric::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear(sf::Color::White);

        Vector position(m_game->width()/2.f, m_game->height()/2.f);

        drawRect(position, 10, sf::Vector3f(1, 0, 0));

        float size = 10;
        Vector v1(position.x - size, position.y + size);
        Vector v2(position.x - size, position.y - size);
        Vector v3(position.x + size, position.y + size);
        Vector v4(position.x + size, position.y - size);
        Vector v5(position.x - size, position.y - size);

        Vector newV1 = matrixMult(v1);
        Vector newV2 = matrixMult(v2);
        Vector newV3 = matrixMult(v3);
        Vector newV4 = matrixMult(v4);
        Vector newV5 = matrixMult(v5);

        glBegin(GL_TRIANGLE_STRIP);
            glColor3f(0, 1, 0);
            glVertex2f(newV1.x, newV1.y);
            glVertex2f(newV2.x, newV2.y);
            glVertex2f(newV3.x, newV3.y);
            glVertex2f(newV4.x, newV4.y);
            glVertex2f(newV5.x, newV5.y);
        glEnd();

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