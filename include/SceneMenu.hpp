#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "Scene.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class SceneMenu : public Scene
{
    public:
        SceneMenu(GameEngine* gameEngine);
        ~SceneMenu();
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;

    private:
        std::shared_ptr<Entity> m_play_starship_button;
        std::shared_ptr<Entity> m_play_platformer_button;
        std::string m_title;
        sf::Text m_menu_title;
        sf::Sprite m_background;
        sf::Music* m_menu_music;
        bool m_debug = false;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        void createButton(const std::string& name, const Vector& position, const std::string& textureName, float width, float height);
        
};

#endif