#ifndef SCENEGRID_HPP
#define SCENEGRID_HPP

#include "Scene.hpp"
#include "Matrix.hpp"


#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class LevelMaker : public Scene
{
    public:
        LevelMaker(GameEngine* gameEngine);
        ~LevelMaker();
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;
        void sAnimation();
        void sMovement();

    private:
        std::shared_ptr<Entity> m_player;
        std::shared_ptr<Entity> m_save_button;
        sf::Sprite              m_background;
        std::string             m_title;
        sf::Text                m_menu_title;
        sf::Clock               m_delta_clock;
        sf::View                m_view;

        const char* tileTypes[6] = { "white_floor", "caution_top_left", "caution_top_right", "caution_bottom_left", "caution_bottom_right", "xbox" };

        bool                    m_debug_render = false;
        bool                    m_collision = false;
        float                   m_debug_value = 5;
        float                   m_dt = 0.01666667;
        float                   m_zoom = 1;
        Vector                  m_mousePos = {};
        Vector                  m_mouseMovePos = {};
        bool                    mouseMiddlePressed = false;

        int                     m_tile_size = 32;
        int                     m_current_tile = 0;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        Vector windowToWorld(const Vector& window) const;
        Vector tileCenter(const sf::Vector2f& mouseCoords) const;
        void createTile(const std::string& textureName, const Vector& position, bool collision);
        void createButton(const std::string& name, const Vector& position, const std::string& textureName, float width, float height);
        bool saveLevel();
};

#endif