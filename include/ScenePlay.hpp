#ifndef SCENEPLAY_HPP
#define SCENEPLAY_HPP

#include "Scene.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class ScenePlay : public Scene
{
    public:
        ScenePlay(GameEngine* gameEngine);
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;
        void sMovement();

    private:
        std::shared_ptr<Entity> m_player;
        std::string m_title;
        sf::Text m_menu_title;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        
};

#endif