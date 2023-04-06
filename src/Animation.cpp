#include "../include/Animation.hpp"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, const sf::Vector2f& size)
    : Animation(name, t, 1, 0, 0, size)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed, int xoffset, const sf::Vector2f& size)
    : m_name(name), m_sprite(t), m_frame_count(frameCount), m_current_frame(0), m_speed(speed), m_xoffset(xoffset), m_size(size)
{
    m_sprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
    m_sprite.setOrigin(m_size.x/2.f, m_size.y/2.f);
}

void Animation::update()
{
    m_current_frame++;

    size_t animFrame = (m_current_frame / m_speed) % m_frame_count;
    sf::IntRect rect(animFrame*m_xoffset, 0, m_size.x, m_size.y);
    m_sprite.setTextureRect(rect);
}

bool Animation::hasEnded() const
{
    return false;
}

const std::string& Animation::getName() const
{
    return m_name;
}

const sf::Vector2f& Animation::getSize() const
{
    return m_size;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}