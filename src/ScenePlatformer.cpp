#include "../include/ScenePlatformer.hpp"
#include "../include/SceneMenu.hpp"
#include "../include/GameEngine.hpp"
#include "../include/Physics.hpp"
#include "../include/DebugShapes.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

ScenePlatformer::ScenePlatformer(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    std::cout << "Loading Platformer\n";
    init();
    m_debug_value = 1;
}

void ScenePlatformer::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Space, "JUMP");
    registerAction(sf::Keyboard::A, "MOVE_LEFT");
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");

    loadAssets();

    m_background.setTexture(m_scene_assets.getTexture("background"));
    m_background.setTextureRect(sf::IntRect(0, 0, m_game->width(), m_game->height()));

    m_player = m_entity_manager.addEntity("player");
    m_player->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    m_player->addComponent<CBoundingBox>(Vector(25, 60));
    m_player->addComponent<CState>();
    m_player->addComponent<CGravity>(Vector(0, 2000));

    createPlatform("platform", m_scene_assets.getTexture("ground_texture"), Vector(m_scene_assets.getTexture("ground_texture").getSize().x, m_game->height()));
    createPlatform("platform", m_scene_assets.getTexture("ground_texture"), Vector(m_scene_assets.getTexture("ground_texture").getSize().x*2, m_game->height()));
    createPlatform("platform", m_scene_assets.getTexture("ground_texture"), Vector(m_scene_assets.getTexture("ground_texture").getSize().x*3, m_game->height()));
    createPlatform("platform", m_scene_assets.getTexture("platform_texture"), Vector(m_scene_assets.getTexture("platform_texture").getSize().x+700, (m_game->height()/2.f)+100));
}

void ScenePlatformer::loadAssets()
{
    m_scene_assets.addTexture("background", "assets/textures/platformer_background.png", true, true);

    m_scene_assets.addTexture("player_idle_texture", "assets/textures/player_idle.png", sf::IntRect(40, 40, 1110, 40), true, false);
    m_scene_assets.addTexture("player_run_texture", "assets/textures/player_run.png", sf::IntRect(40, 40, 1120, 40), true, false);
    m_scene_assets.addTexture("player_jump_texture", "assets/textures/player_jump.png", sf::IntRect(40, 40, 280, 40), true, false);
    m_scene_assets.addTexture("player_attack1_texture", "assets/textures/player_attack.png", sf::IntRect(50, 30, 430, 50), true, false);

    m_scene_assets.addTexture("ground_texture", "assets/textures/scifi_platform_tiles.png", sf::IntRect(0, 192, 224, 224), true, true);
    m_scene_assets.addTexture("platform_texture", "assets/textures/scifi_platform_tiles.png", sf::IntRect(864, 384, 256, 32), true, true);

    m_scene_assets.addAnimation("player_idle_animation", m_scene_assets.getTexture("player_idle_texture"), 10, 5, 120, sf::Vector2f(30, 40));
    m_scene_assets.addAnimation("player_run_animation", m_scene_assets.getTexture("player_run_texture"), 10, 5, 120, sf::Vector2f(40, 40));
    m_scene_assets.addAnimation("player_jump_animation", m_scene_assets.getTexture("player_jump_texture"), 3, 5, 120, sf::Vector2f(40, 40));
    m_scene_assets.addAnimation("player_attack1_animation", m_scene_assets.getTexture("player_attack1_texture"), 4, 4, 120, sf::Vector2f(70, 50));
}

void ScenePlatformer::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
        sCollisions();
        sAnimation();
    }

#ifdef DEBUG
    ImGui::SFML::Update(m_game->window(), m_delta_clock.restart());
    ImGui::Begin("Debug");
    ImGui::Checkbox("Debug", &m_debug);
    ImGui::SliderFloat("player jump", &m_player_stats.jumpForce, 1000, 2000);
    ImGui::End();
#endif

}

void ScenePlatformer::sDoAction(const Action &action)
{
    auto& playerInput = m_player->getComponent<CInput>();
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { onEnd(); }
        if (action.name() == "PAUSE") { setPaused(!m_paused); }
        if (action.name() == "MOVE_LEFT") { playerInput.left = true; }
        if (action.name() == "MOVE_RIGHT") { playerInput.right = true; }
        if (action.name() == "JUMP") { playerInput.up = true; }
        if (action.name() == "MOUSE_LEFT") {playerInput.shoot = true; }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_LEFT") { playerInput.left = false; }
        if (action.name() == "MOVE_RIGHT") { playerInput.right = false; }
        if (action.name() == "JUMP") { playerInput.up = false; playerInput.canJump = true; }
        if (action.name() == "MOUSE_LEFT") {playerInput.shoot = false; }
    }
}

void ScenePlatformer::sMovement()
{
    auto& playerInput = m_player->getComponent<CInput>();
    auto& playerTransform = m_player->getComponent<CTransform>();
    auto& playerState = m_player->getComponent<CState>().m_state;

    Vector acceleration = {};

    if (playerState != "ground")
    {
        acceleration.y = m_player->getComponent<CGravity>().m_gravity.y;
    }

    if (playerInput.left)
    {
        acceleration.x -= m_player_stats.speed;
    }
    else if (playerInput.right)
    {
        acceleration.x += m_player_stats.speed;
    }

    if (playerInput.up && playerInput.canJump && playerState == "ground")
    {
        playerTransform.m_velocity.y -= m_player_stats.jumpForce;
        playerInput.canJump = false;
    }


    Vector dragForce = playerTransform.m_velocity * -m_player_stats.drag;
    acceleration += dragForce;

    playerTransform.m_prev_position = playerTransform.m_position;
    playerTransform.m_velocity += acceleration * m_dt;
    playerTransform.m_position += playerTransform.m_velocity * m_dt + (acceleration*0.5f) * m_dt * m_dt;

}

void ScenePlatformer::sCollisions()
{
    auto& playerTransform = m_player->getComponent<CTransform>();

    m_player->getComponent<CState>().m_state = "air";

    int count = 0;
    for (auto platform : m_entity_manager.getEntities("platform"))
    {
        auto platformTransform = platform->getComponent<CTransform>();

        Vector overlap = rectRectCollision(m_player, platform);
        Vector prevOverlap = prevRectRectCollision(m_player, platform);

        if (overlap.x < 0 || overlap.y < 0) { continue; }

        Vector diff = playerTransform.m_position - platformTransform.m_position;
        Vector shift = {};

        if (prevOverlap.x > 0)
        {
            shift.y = diff.y > 0 ? overlap.y : -overlap.y;
            playerTransform.m_velocity.y = 0;
            if (diff.y < 0)
            {
                m_player->getComponent<CState>().m_state = "ground";
                playerTransform.m_position += platformTransform.m_velocity;
            }
        }
        else if (prevOverlap.y > 0)
        {
            shift.x += diff.x > 0 ? overlap.x : -overlap.x;
            playerTransform.m_velocity.x = 0;
        }
        playerTransform.m_position += shift;
        count++;
    }

    if (playerTransform.m_position.y > m_game->height()-m_player->getComponent<CBoundingBox>().m_half_size.y)
    {
        playerTransform.m_position = Vector(m_game->width()/2.f, m_game->height()/2.f);
    }

    if (playerTransform.m_position.x < m_player->getComponent<CBoundingBox>().m_half_size.x)
    {
        playerTransform.m_position.x = m_player->getComponent<CBoundingBox>().m_half_size.x;
    }
}

void ScenePlatformer::sAnimation()
{
    auto& playerTransform = m_player->getComponent<CTransform>();
    auto playerInput = m_player->getComponent<CInput>();
    auto playerState = m_player->getComponent<CState>().m_state;
    
    if (playerState == "air")
    {
        if (m_player->getComponent<CAnimation>().m_animation.getName() != "player_jump_animation")
        {
            m_player->addComponent<CAnimation>(m_scene_assets.getAnimation("player_jump_animation"), false);
        }
    }
    else if (playerState == "ground")
    {
        if (playerInput.shoot)
        {
            if (m_player->getComponent<CAnimation>().m_animation.getName() != "player_attack1_animation")
            {
                m_player->addComponent<CAnimation>(m_scene_assets.getAnimation("player_attack1_animation"), false);
            }
        }
        else if (playerInput.left || playerInput.right)
        {
            if (m_player->getComponent<CAnimation>().m_animation.getName() != "player_run_animation")
            {
                m_player->addComponent<CAnimation>(m_scene_assets.getAnimation("player_run_animation"), true);
            }
        }
        else
        {
            if (m_player->getComponent<CAnimation>().m_animation.getName() != "player_idle_animation")
            {
                m_player->addComponent<CAnimation>(m_scene_assets.getAnimation("player_idle_animation"), true);
            }
        }
    }



    m_player->getComponent<CAnimation>().m_animation.update();

    for (auto e : m_entity_manager.getEntities())
    {
        if (!e->hasComponent<CAnimation>()) { continue; }

        auto& animation = e->getComponent<CAnimation>();

        if (animation.m_animation.hasEnded() && !animation.m_repeat)
        {
            e->destroy();
        }
        else
        {
            animation.m_animation.update();
        }
    }
}

void ScenePlatformer::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear();

        m_game->window().draw(m_background);

        if (m_debug)
        {

            for (int i = 0; i < m_game->height(); i+=32)
            {
                sf::Vertex* horizontalLine = debugLine(Vector(0, i), Vector(m_game->width(), i), sf::Color::White);
                m_game->window().draw(horizontalLine, 2, sf::Lines);
            }
            for (int i = 0; i < m_game->width(); i+=32)
            {
                sf::Vertex* verticalLine = debugLine(Vector(i, 0), Vector(i, m_game->height()), sf::Color::White);
                m_game->window().draw(verticalLine, 2, sf::Lines);
            }
        }

        auto playerTransform = m_player->getComponent<CTransform>();
        auto playerBox = m_player->getComponent<CBoundingBox>().m_half_size;
        auto& playerAnimation = m_player->getComponent<CAnimation>();

        playerAnimation.m_animation.getSprite().setPosition(sf::Vector2f(playerTransform.m_position.x, playerTransform.m_position.y));
        playerAnimation.m_animation.getSprite().setScale(sf::Vector2f(1.5, 1.5));
        if (playerTransform.m_velocity.x < 0)
        {
            playerAnimation.m_animation.getSprite().setScale(sf::Vector2f(-1.5, 1.5));
        }
        else if (playerTransform.m_velocity.x > 0)
        {
            playerAnimation.m_animation.getSprite().setScale(sf::Vector2f(1.5, 1.5));
        }
        m_game->window().draw(playerAnimation.m_animation.getSprite());

        if (m_debug)
        {
            sf::Vertex* outline = debugRectangle(playerTransform.m_position, playerBox, sf::Color::Red);
            sf::Vertex* centerLineX = debugLine(Vector(playerTransform.m_position.x-playerBox.x, playerTransform.m_position.y), Vector(playerTransform.m_position.x+playerBox.x, playerTransform.m_position.y), sf::Color::Green);
            sf::Vertex* centerLineY = debugLine(Vector(playerTransform.m_position.x, playerTransform.m_position.y-playerBox.y), Vector(playerTransform.m_position.x, playerTransform.m_position.y+playerBox.y), sf::Color::Green);
            m_game->window().draw(outline, 5, sf::LineStrip);
            m_game->window().draw(centerLineX, 2, sf::Lines);
            m_game->window().draw(centerLineY, 2, sf::Lines);
        }


        for (auto e : m_entity_manager.getEntities())
        {
            auto transform = e->getComponent<CTransform>();

            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().m_animation;
                animation.getSprite().setPosition(sf::Vector2f(transform.m_position.x, transform.m_position.y));
                m_game->window().draw(animation.getSprite());
            }
            else
            {
                auto& sprite = e->getComponent<CSprite>().m_sprite;
                sprite.setPosition(sf::Vector2f(transform.m_position.x, transform.m_position.y));
                m_game->window().draw(sprite);
            }

            if (m_debug && e->hasComponent<CBoundingBox>())
            {
                auto box = e->getComponent<CBoundingBox>();
                sf::Vertex* outline = debugRectangle(transform.m_position, box.m_half_size, sf::Color::Red);
                sf::Vertex* centerLineX = debugLine(Vector(transform.m_position.x-box.m_half_size.x, transform.m_position.y), Vector(transform.m_position.x+box.m_half_size.x, transform.m_position.y), sf::Color::Green);
                sf::Vertex* centerLineY = debugLine(Vector(transform.m_position.x, transform.m_position.y-box.m_half_size.y), Vector(transform.m_position.x, transform.m_position.y+box.m_half_size.y), sf::Color::Green);
                m_game->window().draw(centerLineX, 2, sf::Lines);
                m_game->window().draw(centerLineY, 2, sf::Lines);
                m_game->window().draw(outline, 5, sf::LineStrip);
            }

        }
    }
    else
    {
    }

#ifdef DEBUG
    ImGui::SFML::Render(m_game->window());
#endif

}

void ScenePlatformer::createPlatform(const std::string& name, const sf::Texture& t, const Vector& position)
{
    auto platform = m_entity_manager.addEntity(name);
    platform->addComponent<CSprite>(t);
    sf::IntRect spriteRect = platform->getComponent<CSprite>().m_sprite.getTextureRect();
    platform->addComponent<CTransform>(Vector(position.x-spriteRect.width/2.f, position.y - spriteRect.height/2.f));
    platform->addComponent<CBoundingBox>(spriteRect);
}

void ScenePlatformer::onEnd()
{
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

ScenePlatformer::~ScenePlatformer()
{
}