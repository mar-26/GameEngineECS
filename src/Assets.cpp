#include "../include/Assets.hpp"
#include <SFML/Graphics/Texture.hpp>

#include <fstream>
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