#ifndef SCENESTATICANALYSIS_HPP
#define SCENESTATICANALYSIS_HPP

#include "Scene.hpp"
#include "Matrix.hpp"


#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class SceneStaticAnalysis : public Scene
{
    public:
        SceneStaticAnalysis(GameEngine* gameEngine);
        ~SceneStaticAnalysis();
        void sRender() override;
        void sDoAction(const Action& action) override;
        void sCollisions() override;
        void sAnimation();
        void sMovement();

    private:
        std::shared_ptr<Entity> m_player;
        sf::Sprite              m_background;
        std::string             m_title;
        sf::Text                m_menu_title;
        sf::Clock               m_delta_clock;
        sf::View                m_view;
        sf::View                m_hud_view;
        bool                    m_debug = false;
        float                   m_debug_value = 5;
        float                   m_dt = 0.01666667;
        float                   m_zoom = 1;
        Vector                  m_mousePos = {};
        Vector                  m_mouseMovePos = {};
        bool                    mouseLeftPressed = false;
        
        void init();
        void update() override;
        void onEnd() override;
        void loadAssets() override;
        Vector windowToWorld(const Vector& window) const;
};

#endif