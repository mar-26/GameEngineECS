#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Sound.hpp>

#include "Vector.hpp"

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
        float m_angle = 0;

        CTransform() {}
        CTransform(const Vector& pos) : m_position(pos) {}
        CTransform(const Vector& pos, const Vector& vel, float angle)
            : m_position(pos), m_velocity(vel), m_angle(angle) {}
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

class CSoundEffect : public Component
{
    public:
        sf::Sound m_sound;

        CSoundEffect() {}
        CSoundEffect(sf::Sound sound)
            : m_sound(sound) {}
};

class CBoundingCircle : public Component
{
    public:
        float m_radius;
        Vector m_position;
        sf::Vertex m_outline[11];

        CBoundingCircle() {}
        CBoundingCircle(float r, Vector pos)
            : m_radius(r), m_position(pos) {
                int i = 0; 
                for (float a = 0; a < 6.28; a += 0.628)
                {
                    m_outline[i] = sf::Vector2f(m_radius*cos(a)+m_position.x, m_radius*sin(a)+m_position.y);
                    m_outline[i].color = sf::Color::Red;
                    i++;
                }
            }

};

class CBoundingBox : public Component
{
    public:
        sf::FloatRect m_box;
        Vector m_half_size;
        sf::Vertex m_outline[5];

        CBoundingBox() {}
        CBoundingBox(sf::FloatRect rect, Vector halfSize)
            : m_box(rect), m_half_size(halfSize) {
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