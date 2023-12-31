#include "../include/Assets.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <iostream>

#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

Assets::Assets()
{
}

void Assets::addTexture(const std::string& name, const std::string& fileName, bool smooth, bool repeted)
{
    m_textures[name] = sf::Texture();
    if (!m_textures[name].loadFromFile(fileName))
    {
        std::cerr << "\tCould not load texture file: " << fileName << std::endl;
        m_textures.erase(name);
    }
    else
    {
        m_textures[name].setSmooth(smooth);
        m_textures[name].setRepeated(repeted);
        std::cout << "\tLoaded Texture: " << fileName << std::endl;
    }
}


void Assets::addTexture(const std::string& name, const std::string& fileName, const sf::IntRect rect, bool smooth, bool repeted)
{
    m_textures[name] = sf::Texture();
    if (!m_textures[name].loadFromFile(fileName, rect))
    {
        std::cerr << "\tCould not load texture file: " << fileName << std::endl;
        m_textures.erase(name);
    }
    else
    {
        m_textures[name].setSmooth(smooth);
        m_textures[name].setRepeated(repeted);
        std::cout << "\tLoaded Texture: " << fileName << std::endl;
    }
}

void Assets::addAnimation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed, int xoffset, const sf::Vector2f& size)
{
    m_animation[name] = Animation(name, t, frameCount, speed, xoffset, size);
    std::cout << "\tLoaded Animation: " << name << std::endl;
}

void Assets::addSound(const std::string& name, const std::string& path)
{
    // The sound buffer needs to stay alive.
    // Not sure if this is the best way to do it though
    static sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(path))
    {
        std::cerr << "\tCould not load sound file: " << path << std::endl;
    }
    else
    {
        m_sounds[name] = sf::Sound(buffer);
        std::cout << "\tLoaded Sound: " << path << std::endl;
    }
}

void Assets::addMusic(const std::string& name, const std::string& path)
{
    m_music[name] = new sf::Music();
    if (!m_music[name]->openFromFile(path))
    {
        std::cerr << "\tCould not load music file: " << path << std::endl;
    }
    else
    {
        std::cout << "\tLoaded Music: " << path << std::endl;
    }
}

void Assets::addFont(const std::string& name, const std::string& path)
{
    m_fonts[name] = sf::Font();
    if (!m_fonts[name].loadFromFile(path))
    {
        std::cerr << "\tCould not load font file: " << path << std::endl;
        m_fonts.erase(name);
    }
    else
    {
        std::cout << "\tLoaded Font: " << path << std::endl;
    }
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
    ASSERT(m_textures.find(name) != m_textures.end(), "Could not load " + name + " texture");
    return m_textures.at(name);
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
    ASSERT(m_sounds.find(name) != m_sounds.end(), "Could not load " + name + " sound");
    return m_sounds.at(name);
}

sf::Music* Assets::getMusic(const std::string& name) const
{
    ASSERT(m_music.find(name) != m_music.end(), "Could not load " + name + " music");
    return m_music.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
    ASSERT(m_fonts.find(name) != m_fonts.end(), "Could not load " + name + " font");
    return m_fonts.at(name);
}

const Animation Assets::getAnimation(const std::string& name) const
{
    ASSERT(m_animation.find(name) != m_animation.end(), "Could not load " + name + " animation");
    return m_animation.at(name);
}