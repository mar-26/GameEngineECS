#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <string>

class Animation
{
    public:
        Animation();
        Animation(const std::string& name, const sf::Texture& t, const sf::Vector2f& size);
        Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed, int xoffset, const sf::Vector2f& size);

        void update();
        bool hasEnded() const;
        const std::string& getName() const;
        const sf::Vector2f& getSize() const;
        sf::Sprite& getSprite();

    private:
        std::string  m_name;
        sf::Sprite   m_sprite;
        size_t       m_frame_count;
        size_t       m_current_frame;
        size_t       m_speed;
        int          m_xoffset;
        sf::Vector2f m_size;
};

#endif