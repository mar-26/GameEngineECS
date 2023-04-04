#include "../include/SceneStarship.hpp"
#include "../include/GameEngine.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

SceneStarship::SceneStarship(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
    m_debug_value = 1;
}

void SceneStarship::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Space, "SHOOT");
    registerAction(sf::Keyboard::W, "MOVE_FORWARD");
    registerAction(sf::Keyboard::S, "MOVE_BACKWARD");
    registerAction(sf::Keyboard::A, "TURN_LEFT");
    registerAction(sf::Keyboard::D, "TURN_RIGHT");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("space_background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));

    m_player = m_entity_manager.addEntity("player");
    m_player->addComponent<CInput>();
    m_player->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    m_player->addComponent<CSprite>(m_scene_assets.getTexture("player_ship"));
    m_player->addComponent<CBoundingCircle>(17);
    m_player->addComponent<CSoundEffect>(m_scene_assets.getSound("bullet_noise"));

    auto enemy = m_entity_manager.addEntity("enemy");
    enemy->addComponent<CTransform>(Vector(300, 70));
    enemy->addComponent<CSprite>(m_scene_assets.getTexture("enemy_ship"));
    enemy->addComponent<CBoundingCircle>(17);
}

void SceneStarship::loadAssets()
{
    m_scene_assets.addTexture("space_background", "assets/textures/space_background.png", true, true);
    m_scene_assets.addTexture("player_ship", "assets/textures/modular_ships.png", sf::IntRect(80, 320, 32, 32), true, true);
    m_scene_assets.addTexture("enemy_ship", "assets/textures/modular_ships.png", sf::IntRect(40, 328, 32, 28), true, true);
    m_scene_assets.addTexture("red_bullet", "assets/textures/beams.png", sf::IntRect(39, 143, 14, 18), true, true);
    m_scene_assets.addSound("bullet_noise", "assets/sounds/sfx_wpn_laser.ogg");
}

void SceneStarship::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
        sCollisions();
    }

#ifdef DEBUG
    ImGui::SFML::Update(m_game->window(), m_delta_clock.restart());
    ImGui::Begin("Debug");
    ImGui::Checkbox("Debug", &m_debug);
    ImGui::Text("angle = %f", m_player->getComponent<CTransform>().m_angle);
    ImGui::SliderFloat("debug value", &m_debug_value, 0.1, 1);
    ImGui::End();
#endif

}

void SceneStarship::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        Vector playerPosition = {};
        Vector mousePos = {};
        Vector mousePlayerDiff = {};
        float angle = 0;

        if (action.name() == "QUIT") { onEnd(); }
        if (action.name() == "MOVE_FORWARD") { m_player->getComponent<CInput>().up = true; }
        if (action.name() == "MOVE_BACKWARD") { m_player->getComponent<CInput>().down = true; }
        if (action.name() == "TURN_LEFT") {
            m_player->getComponent<CInput>().left = true;
        }
        if (action.name() == "TURN_RIGHT") {
            m_player->getComponent<CInput>().right = true;
        }
        if (action.name() == "SHOOT") { m_player->getComponent<CInput>().shoot = true; }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_FORWARD") { m_player->getComponent<CInput>().up = false; }
        if (action.name() == "MOVE_BACKWARD") { m_player->getComponent<CInput>().down = false; }
        if (action.name() == "TURN_LEFT") { m_player->getComponent<CInput>().left = false; }
        if (action.name() == "TURN_RIGHT") { m_player->getComponent<CInput>().right = false; }
        if (action.name() == "SHOOT")
        {
            m_player->getComponent<CInput>().shoot = false;
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void SceneStarship::sMovement()
{
    auto& transform = m_player->getComponent<CTransform>();
    Vector previousVelocity = transform.m_velocity;
    auto playerInput = m_player->getComponent<CInput>();

    // figure out how to forward and backward in the direction the ship is facing 
    float playerAngle = (transform.m_angle-90) * (M_PI/180);
    if (playerInput.up) { previousVelocity = Vector(m_player_stats.speed*cos(playerAngle), m_player_stats.speed*sin(playerAngle)); }

    transform.m_velocity = previousVelocity;
    transform.m_position += transform.m_velocity;
    
    if (m_player->getComponent<CInput>().shoot && m_player->getComponent<CInput>().canShoot)
    {
        m_player->getComponent<CSoundEffect>().m_sound.play();
        m_player->getComponent<CInput>().canShoot = false;
        spawnBullet(m_player, playerAngle);
    }

    auto bullets = m_entity_manager.getEntities("bullet");
    float bulletAngle = playerAngle;

    for (int i = 0; i < (int)bullets.size(); i++)
    {
        auto& bulletTransform = bullets[i]->getComponent<CTransform>();
        bulletTransform.m_position += bulletTransform.m_velocity;
    }
}

void SceneStarship::sCollisions()
{
    auto& playerTransform = m_player->getComponent<CTransform>();

    if (playerTransform.m_position.x >= m_game->width())
    {
        playerTransform.m_position.x = 1;
    }
    if (playerTransform.m_position.x <= 0)
    {
        playerTransform.m_position.x = m_game->width()-1;
    }
    if (playerTransform.m_position.y >= m_game->height())
    {
        playerTransform.m_position.y = 1;
    }
    if (playerTransform.m_position.y <= 0)
    {
        playerTransform.m_position.y = m_game->height()-1;
    }

    // enemy runs into player
    for (auto e : m_entity_manager.getEntities("enemy"))
    {
        if (circleCircleHit(e, m_player))
        {
        }
    }

    for (auto enemy : m_entity_manager.getEntities("enemy"))
    {
        for (auto bullet : m_entity_manager.getEntities("bullet"))
        {
            if (circleCircleHit(bullet, enemy))
            {
                enemy->destroy();
            }
        }
    }
}

float lerp(float v0, float v1, float t)
{
    return (1-t) * v0 + t * v1;
}

void SceneStarship::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear();
    }
    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    m_game->window().draw(m_background);


    // draw player
    auto& playerSprite = m_player->getComponent<CSprite>().m_sprite;
    auto playerTransform = m_player->getComponent<CTransform>();
    for (float t = 0; t < 1; t+=0.1)
    {
        if (m_player->getComponent<CInput>().left)
        {
            playerSprite.setRotation(lerp(playerTransform.m_angle, playerTransform.m_angle-m_player_stats.turnAngle, t));
            m_player->getComponent<CTransform>().m_angle += -m_player_stats.turnAngle;
        }
        else if (m_player->getComponent<CInput>().right)
        {
            playerSprite.setRotation(lerp(playerTransform.m_angle, playerTransform.m_angle+m_player_stats.turnAngle, t));
            m_player->getComponent<CTransform>().m_angle += m_player_stats.turnAngle;
        }

        // debug for player orientation
        if (m_debug)
        {
            sf::Vertex* outline = boundingCircle(m_player->getComponent<CBoundingCircle>().m_radius, playerTransform.m_position, sf::Color::Green);
            m_game->window().draw(outline, 12, sf::LineStrip);

            auto playerAngle = (m_player->getComponent<CTransform>().m_angle-90) * (M_PI/180);
            sf::Vertex* normalLine = debugLine(playerTransform.m_position, Vector(playerSprite.getTextureRect().height*2*cos(playerAngle)+playerTransform.m_position.x, playerSprite.getTextureRect().height*2 * sin(playerAngle) + playerTransform.m_position.y), sf::Color::Red);
            m_game->window().draw(normalLine, 2, sf::Lines);
        }
        playerSprite.setPosition(sf::Vector2f(playerTransform.m_position.x, playerTransform.m_position.y));
        m_game->window().draw(playerSprite);
    }

    for (auto e : m_entity_manager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();

        if (e->hasComponent<CSprite>())
        {
            auto& sprite = e->getComponent<CSprite>().m_sprite;
            sprite.setPosition(transform.m_position.x, transform.m_position.y);
            sprite.setRotation(transform.m_angle);
            m_game->window().draw(sprite);

        }

        if (e->hasComponent<CBoundingCircle>() && m_debug)
        {
            auto circle = e->getComponent<CBoundingCircle>();
            sf::Vertex* outline = boundingCircle(circle.m_radius, transform.m_position, sf::Color::Red);
            m_game->window().draw(outline, 12, sf::LineStrip);
        }
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void SceneStarship::spawnBullet(std::shared_ptr<Entity> entity, float angle)
{
    float bulletSpeed = 10;
    auto bullet = m_entity_manager.addEntity("bullet");
    bullet->addComponent<CTransform>(entity->getComponent<CTransform>().m_position, Vector(bulletSpeed*cos(angle), bulletSpeed*sin(angle)), m_player->getComponent<CTransform>().m_angle);
    bullet->addComponent<CSprite>(m_scene_assets.getTexture("red_bullet"));
    bullet->addComponent<CBoundingCircle>(5);
}

void SceneStarship::onEnd()
{
    m_game->quit();
}

SceneStarship::~SceneStarship()
{
}