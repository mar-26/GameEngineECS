#include "../include/GameEngine.hpp"

#include "../include/Action.hpp"
#include "../include/SceneMenu.hpp"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Window/Event.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>

GameEngine::GameEngine(int w, int h, const std::string& windowTitle)
{
    m_width = w;
    m_height = h;
    m_window_title = windowTitle;
    m_running = true;

    init();
}

void GameEngine::init()
{
    m_window.create(sf::VideoMode(m_width, m_height), m_window_title);
    m_window.setFramerateLimit(60);
    m_window.setActive(true);

#ifdef DEBUG
    ImGui::SFML::Init(m_window);
#endif

    loadAssets();

    changeScene("MENU", std::make_shared<SceneMenu>(this));
}

void GameEngine::loadAssets()
{

}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::update()
{
    if (!isRunning())
    {
        return;
    }

    if (m_scene_map.empty())
    {
        return;
    }

    sUserInput();
    currentScene()->update();
    currentScene()->sRender();

    m_window.display();
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {

#ifdef DEBUG
        ImGui::SFML::ProcessEvent(m_window, event);
#endif

        if (event.type == sf::Event::Closed)
        {
            quit();
        }
        else if (event.type == sf::Event::Resized)
        {
            glViewport(0, 0, event.size.width, event.size.height);
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // if there is no aciton for the key then ignore it
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }

            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }

        auto mousePos = sf::Mouse::getPosition(m_window);
        Vector mpos = Vector(mousePos.x, mousePos.y);

        if (event.type == sf::Event::MouseButtonPressed)
        {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                {
                    currentScene()->sDoAction(Action("MOUSE_LEFT", "START", mpos));
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->sDoAction(Action("MOUSE_RIGHT", "START", mpos));
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->sDoAction(Action("MOUSE_MIDDLE", "START", mpos));
                    break;
                }
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                {
                    currentScene()->sDoAction(Action("MOUSE_LEFT", "END", mpos));
                    break;
                }
                case sf::Mouse::Right:
                {
                    currentScene()->sDoAction(Action("MOUSE_RIGHT", "END", mpos));
                    break;
                }
                case sf::Mouse::Middle:
                {
                    currentScene()->sDoAction(Action("MOUSE_MIDDLE", "END", mpos));
                    break;
                }
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {
            currentScene()->sDoAction(Action("MOUSE_MOVE", "START", Vector(event.mouseMove.x, event.mouseMove.y)));
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            currentScene()->sDoAction(Action("MOUSE_WHEEL", "START", Vector(event.mouseWheel.x, event.mouseWheel.y), event.mouseWheel.delta));
        }
    }
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (scene)
    {
        m_scene_map[sceneName] = scene;
    }
    else
    {
        if (m_scene_map.find(sceneName) == m_scene_map.end())
        {
            std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
            return;
        }
    }

    if (endCurrentScene)
    {
        m_scene_map.erase(m_scene_map.find(m_current_scene));
    }

    m_current_scene = sceneName;
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_scene_map[m_current_scene];
}

void GameEngine::quit()
{
    m_running = false;
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

bool GameEngine::isRunning()
{
    return m_running && m_window.isOpen();
}

const Assets& GameEngine::assets() const
{
    return m_game_assets;
}

const int GameEngine::width() const
{
    return m_width;
}

const int GameEngine::height() const
{
    return m_height;
}

GameEngine::~GameEngine()
{

#ifdef DEBUG
    ImGui::SFML::Shutdown();
#endif 

}