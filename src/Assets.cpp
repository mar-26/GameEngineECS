#include "../include/Assets.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <iostream>
#include <assert.h>

Assets::Assets()
{
}

void Assets::addTexture(const std::string& name, const std::string& fileName, bool smooth, bool repeted)
{
    m_textures[name] = sf::Texture();
    if (!m_textures[name].loadFromFile(fileName))
    {
        std::cerr << "Could not load texture file: " << fileName << std::endl;
        m_textures.erase(name);
    }
    else
    {
        m_textures[name].setSmooth(smooth);
        m_textures[name].setRepeated(repeted);
        std::cout << "Loaded Texture: " << fileName << std::endl;
    }
}


void Assets::addTexture(const std::string& name, const std::string& fileName, const sf::IntRect rect, bool smooth, bool repeted)
{
    m_textures[name] = sf::Texture();
    if (!m_textures[name].loadFromFile(fileName, rect))
    {
        std::cerr << "Could not load texture file: " << fileName << std::endl;
        m_textures.erase(name);
    }
    else
    {
        m_textures[name].setSmooth(smooth);
        m_textures[name].setRepeated(repeted);
        std::cout << "Loaded Texture: " << fileName << std::endl;
    }
}

void Assets::addSound(const std::string& name, const std::string& path)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(path))
    {
        std::cerr << "Could not load sound file: " << path << std::endl;
    }
    else
    {
        m_sounds[name] = sf::Sound();
        m_sounds[name].setBuffer(buffer);
        std::cout << "Loaded Sound: " << path << std::endl;
    }
}

void Assets::addMusic(const std::string& name, const std::string& path)
{
    m_music[name] = new sf::Music();
    if (!m_music[name]->openFromFile(path))
    {
        std::cerr << "Could not load music file: " << path << std::endl;
    }
    else
    {
        std::cout << "Loaded Music: " << path << std::endl;
    }
}

void Assets::addFont(const std::string& name, const std::string& path)
{
    m_fonts[name] = sf::Font();
    if (!m_fonts[name].loadFromFile(path))
    {
        std::cerr << "Could not load font file: " << path << std::endl;
        m_fonts.erase(name);
    }
    else
    {
        std::cout << "Loaded Font: " << path << std::endl;
    }
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
    assert(m_textures.find(name) != m_textures.end());
    return m_textures.at(name);
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
    assert(m_sounds.find(name) != m_sounds.end());
    return m_sounds.at(name);
}

sf::Music* Assets::getMusic(const std::string& name) const
{
    assert(m_music.find(name) != m_music.end());
    return m_music.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
    assert(m_fonts.find(name) != m_fonts.end());
    return m_fonts.at(name);
}

void Assets::clearTextures()
{
    m_textures.clear();
}

void Assets::clearSounds()
{
    m_sounds.clear();
}

void Assets::clearFonts()
{
    m_fonts.clear();
}