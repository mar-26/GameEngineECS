#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "Scene.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class SceneMenu : public Scene
{
    public:
        SceneMenu(GameEngine* gameEngine);
        void sRender() override;
        void sDoAction(const Action& action) override;

    private:
        std::string m_title;
        sf::Text m_menu_title;
        sf::Sprite m_background;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        
};

#endif