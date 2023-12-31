#ifndef SCENEPLATFORMER_HPP
#define SCENEPLATFORMER_HPP

#include "Scene.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class ScenePlatformer : public Scene
{
    public:
        ScenePlatformer(GameEngine* gameEngine);
        ~ScenePlatformer();
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
        std::string             m_level_file;
        bool                    m_debug = false;
        float                   m_debug_value = 5;
        float                   m_dt = 0.01666667;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        bool loadLevel(const std::string& fileName);
        void createTile(const std::string& textureName, const Vector& position, bool collision);
};

#endif