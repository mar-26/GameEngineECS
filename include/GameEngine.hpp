#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Assets.hpp"
#include "Scene.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFML/System/Clock.hpp>
#include <map>
#include <memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
    public:
        GameEngine(int w, int h, const std::string& windowTitle);
        ~GameEngine();

        void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

        void run();
        void quit();

        const int width() const;
        const int height() const;

        sf::RenderWindow& window();
        const Assets& assets() const;
        bool isRunning();

    private:
        sf::RenderWindow m_window;
        Assets           m_game_assets;
        SceneMap         m_scene_map;
        std::string      m_current_scene;
        bool             m_running;
        std::string      m_window_title;
        int              m_width;
        int              m_height;

        void init();
        void loadAssets();
        void update();
        void sUserInput();
        std::shared_ptr<Scene> currentScene();
};

#endif