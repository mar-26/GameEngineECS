#ifndef SCENEISOMETRIC_HPP
#define SCENEISOMETRIC_HPP

#include "Scene.hpp"
#include "Matrix.hpp"


#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class SceneIsometric : public Scene
{
    public:
        SceneIsometric(GameEngine* gameEngine);
        ~SceneIsometric();
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;
        void sAnimation();
        void sMovement();

        struct PlayerStats
        {
            float speed = 1000;
            float drag = 7;
            float jumpForce = 2000;
        };

    private:
        struct PlayerStats     m_player_stats;
        std::shared_ptr<Entity> m_player;
        sf::Sprite              m_background;
        std::string             m_title;
        sf::Text                m_menu_title;
        sf::Clock               m_delta_clock;
        bool                    m_debug = false;
        float                   m_debug_value = 5;
        float                   m_scale = 1;
        float                   m_skew = 0;
        float                   m_angle = 0;
        float                   m_dt = 0.01666667;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
};

#endif