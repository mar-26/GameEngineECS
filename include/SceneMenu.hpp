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
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;

    private:
        std::shared_ptr<Entity> m_menu_button;
        std::string m_title;
        sf::Text m_menu_title;
        sf::Sprite m_background;
        bool m_debug = false;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        
};

#endif