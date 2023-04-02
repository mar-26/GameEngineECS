#include "../include/ScenePlay.hpp"
#include "../include/GameEngine.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui-sfml/imgui-SFML.h"


#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Keyboard.hpp>

ScenePlay::ScenePlay(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void ScenePlay::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Space, "BOOST");

    loadAssets();

    auto spaceBackground = m_entity_manager.addEntity("space_background");
    spaceBackground->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    spaceBackground->addComponent<CSprite>(m_scene_assets.getTexture("space_background"), sf::IntRect(0, 0, m_game->width(), m_game->height()));

    m_player = m_entity_manager.addEntity("player");
    m_player->addComponent<CInput>();
    m_player->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    m_player->addComponent<CSprite>(m_scene_assets.getTexture("ship_a"));
    m_player->addComponent<CSoundEffect>(m_scene_assets.getSound("bullet_noise"));
}

void ScenePlay::loadAssets()
{
    m_scene_assets.addTexture("space_background", "assets/textures/space_background.png", true, true);
    m_scene_assets.addTexture("ship_a", "assets/textures/modular_ships.png", sf::IntRect(80, 320, 32, 32), true, true);
    m_scene_assets.addTexture("red_bullet", "assets/textures/beams.png", sf::IntRect(39, 143, 14, 18), true, true);
    m_scene_assets.addSound("bullet_noise", "assets/sounds/sfx_wpn_laser.ogg");
}

void ScenePlay::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
        sCollisions();
    }
    ImGui::SFML::Update(m_game->window(), m_delta_clock.restart());
    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();
}

void ScenePlay::sRender()
{
    if (!m_paused)
    {
        m_game->window().clear();
    }
    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    for (auto e : m_entity_manager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();

        if (e->hasComponent<CSprite>())
        {
            auto& sprite = e->getComponent<CSprite>().m_sprite;
            sprite.setPosition(transform.m_position.x, transform.m_position.y);
            m_game->window().draw(sprite);
        }
    }
    ImGui::SFML::Render(m_game->window());
}

void ScenePlay::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        Vector playerPosition = {};
        Vector mousePos = {};
        Vector mousePlayerDiff = {};
        float angle = 0;

        if (action.name() == "QUIT") { onEnd(); }
        if (action.name() == "BOOST") { m_player->getComponent<CInput>().up = true; }
        if (action.name() == "MOUSE_MOVE")
        {

            playerPosition = m_player->getComponent<CTransform>().m_position;
            mousePos = Vector(action.pos().x, action.pos().y);
            mousePlayerDiff = mousePos-playerPosition;

            angle = atan2(mousePlayerDiff.y, mousePlayerDiff.x);

            m_player->getComponent<CTransform>().m_angle = angle;

            angle = angle * (180/M_PI) + 90;

            m_player->getComponent<CSprite>().m_sprite.setRotation(angle);
        }
        if (action.name() == "MOUSE_LEFT")
        {
            m_player->getComponent<CSoundEffect>().m_sound.play();
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "BOOST") { m_player->getComponent<CInput>().up = false; }
        if (action.name() == "MOUSE_LEFT")
        {
            m_player->getComponent<CInput>().shoot = false;
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void ScenePlay::sMovement()
{
    auto& transform = m_player->getComponent<CTransform>();
    Vector previousVelocity = transform.m_velocity;
    auto playerInput = m_player->getComponent<CInput>();

    if (playerInput.up) { previousVelocity = Vector(4*cos(transform.m_angle), 3*sin(transform.m_angle)); }

    transform.m_velocity = previousVelocity;
    transform.m_position += transform.m_velocity;

}

void ScenePlay::sCollisions()
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
}

void ScenePlay::onEnd()
{
    m_game->quit();
}

ScenePlay::~ScenePlay()
{
}