#include "../include/Animation.hpp"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t)
    : Animation(name, t, 1, 0)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
    : m_name(name), m_sprite(t), m_frame_count(frameCount), m_speed(speed)
{

}

void Animation::update()
{
    m_current_frame++;
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