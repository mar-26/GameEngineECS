#include "../include/ScenePlay.hpp"
#include "../include/GameEngine.hpp"

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
    registerAction(sf::Keyboard::W, "MOVE_UP");
    registerAction(sf::Keyboard::S, "MOVE_DOWN");
    registerAction(sf::Keyboard::A, "MOVE_LEFT");
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");

    loadAssets();

    auto spaceBackground = m_entity_manager.addEntity("space_background");
    spaceBackground->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    spaceBackground->addComponent<CSprite>(m_scene_assets.getTexture("space_background"), sf::IntRect(0, 0, m_game->width(), m_game->height()));

    m_player = m_entity_manager.addEntity("player");
    m_player->addComponent<CInput>();
    m_player->addComponent<CTransform>(Vector(m_game->width()/2.f, m_game->height()/2.f));
    m_player->addComponent<CSprite>(m_scene_assets.getTexture("ship_a"));
}

void ScenePlay::loadAssets()
{
    m_scene_assets.addTexture("space_background", "assets/textures/space_background.png", true, true);
    m_scene_assets.addTexture("ship_a", "assets/textures/modular_ships.png", sf::IntRect(80, 320, 32, 32), true, true);
    m_scene_assets.addTexture("red_bullet", "assets/textures/beams.png", sf::IntRect(39, 143, 14, 18), true, true);
}

void ScenePlay::update()
{
    m_entity_manager.update();
    if (!m_paused)
    {
        sMovement();
    }
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
        if (action.name() == "MOVE_UP") { m_player->getComponent<CInput>().up = true; }
        if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = true; }
        if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true; }
        if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }
        if (action.name() == "MOUSE_MOVE")
        {

            playerPosition = m_player->getComponent<CTransform>().m_position;
            mousePos = Vector(action.pos().x, action.pos().y);
            mousePlayerDiff = mousePos-playerPosition;

            angle = atan2(mousePlayerDiff.y, mousePlayerDiff.x) * (180/M_PI) + 90;

            m_player->getComponent<CSprite>().m_sprite.setRotation(angle);
        }
        if (action.name() == "MOUSE_LEFT")
        {
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_UP") { m_player->getComponent<CInput>().up = false; }
        if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = false; }
        if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false; }
        if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false; }
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

    if (playerInput.up) { previousVelocity.y -= 1; }
    if (playerInput.down) { previousVelocity.y += 1; }
    if (playerInput.left) { previousVelocity.x -= 1; }
    if (playerInput.right) { previousVelocity.x += 1; }

    transform.m_velocity = previousVelocity;
    transform.m_position += transform.m_velocity;

}

void ScenePlay::onEnd()
{
    m_game->quit();
}