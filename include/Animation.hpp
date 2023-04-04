#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <string>

class Animation
{
    public:
        Animation();
        Animation(const std::string& name, const sf::Texture& t);
        Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

        void update();
        bool hasEnded() const;
        const std::string& getName() const;
        const sf::Vector2f& getSize() const;
        sf::Sprite& getSprite();

    private:
        sf::Sprite   m_sprite;
        size_t       m_frame_count;
        size_t       m_current_frame;
        size_t       m_speed;
        sf::Vector2f m_size;
        std::string  m_name;
};

#endif