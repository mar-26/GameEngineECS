#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>

#include <map>

class Assets
{
    public:
        Assets();

        void addTexture(const std::string& name, const std::string& fileName, bool smooth, bool repeted);
        void addTexture(const std::string& name, const std::string& fileName, const sf::IntRect rect, bool smooth, bool repeted);
        void addSound(const std::string& name, const std::string& path);
        void addMusic(const std::string& name, const std::string& path);
        void addFont(const std::string& name, const std::string& path);

        const sf::Texture& getTexture(const std::string& name) const;
        const sf::Sound& getSound(const std::string& name) const;
        sf::Music* getMusic(const std::string& name) const;
        const sf::Font& getFont(const std::string& name) const;

        void clearTextures();
        void clearSounds();
        void clearFonts();

    private:
        std::map<std::string, sf::Font>    m_fonts;
        std::map<std::string, sf::Sound>   m_sounds;
        std::map<std::string, sf::Music*>   m_music;
        std::map<std::string, sf::Texture> m_textures;

};

#endif