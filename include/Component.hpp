#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Vector.h"

class Component
{
    public:
        bool has = false;
};

class CTransform : public Component
{
    public:
        Vector m_position = {};
        Vector m_velocity = {};

        CTransform() {}
        CTransform(const Vector& pos) : m_position(pos) {}
        CTransform(const Vector& pos, const Vector& vel)
            : m_position(pos), m_velocity(vel) {}
};

class CSprite : public Component
{
    public:
        sf::Sprite m_sprite;

        CSprite() {}
        CSprite(const sf::Texture& texture)
            : m_sprite(texture) {
                m_sprite.setOrigin(m_sprite.getTextureRect().width/2.f, m_sprite.getTextureRect().height/2.f);
            }
        CSprite(const sf::Texture& texture, const sf::IntRect& rect)
            : m_sprite(texture, rect) {
                m_sprite.setOrigin(rect.width/2.f, rect.height/2.f);
            }
};

class CInput : public Component
{
    public:
        bool up    = false;
        bool down  = false;
        bool left  = false;
        bool right = false;
        bool shoot = false;
        bool canShoot = true;
};

class CBoundingBox : public Component
{
    public:
        sf::FloatRect m_box;
        sf::Vertex m_outline[5];

        CBoundingBox() {}
        CBoundingBox(sf::FloatRect rect)
            : m_box(rect) {
                m_outline[0] = sf::Vector2f(m_box.left, m_box.top);
                m_outline[1] = sf::Vector2f(m_box.left+m_box.width, m_box.top);
                m_outline[2] = sf::Vector2f(m_box.left+m_box.width, m_box.top+m_box.height);
                m_outline[3] = sf::Vector2f(m_box.left, m_box.top+m_box.height);
                m_outline[4] = sf::Vector2f(m_box.left, m_box.top);
                m_outline[0].color = sf::Color::Red;
                m_outline[1].color = sf::Color::Red;
                m_outline[2].color = sf::Color::Red;
                m_outline[3].color = sf::Color::Red;
                m_outline[4].color = sf::Color::Red;
            }
};

#endif