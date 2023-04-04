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

        CBoundingCircle() {}
        CBoundingCircle(float r)
            : m_radius(r) {}

};

class CBoundingBox : public Component
{
    public:
        sf::FloatRect m_box;
        Vector m_half_size;

        CBoundingBox() {}
        CBoundingBox(sf::FloatRect rect, Vector halfSize)
            : m_box(rect), m_half_size(halfSize) {}
};

#endif