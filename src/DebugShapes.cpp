#include "../include/DebugShapes.hpp"


sf::Vertex* debugCircle(float radius, const Vector& position, sf::Color color)
{
    sf::Vertex* outline = new sf::Vertex[12];
    int i = 0; 
    for (float a = 0; a < 6.28; a += 0.628)
    {
        outline[i] = sf::Vector2f(radius*cos(a)+position.x, radius*sin(a)+position.y);
        outline[i].color = color;
        i++;
    }
    outline[i] = sf::Vector2f(radius*cos(0)+position.x, radius*sin(0)+position.y);
    outline[i].color = color;
    return outline;
}

sf::Vertex* debugLine(const Vector& p0, const Vector& p1, sf::Color color)
{
    sf::Vertex* normalLine = new sf::Vertex[2];
    normalLine[0] = sf::Vector2f(p0.x, p0.y);
    normalLine[1] = sf::Vector2f(p1.x, p1.y);
    normalLine[0].color = color;
    normalLine[1].color = color;
    return normalLine;
}

sf::Vertex* debugRectangle(const Vector& p0, const Vector& halfsize, sf::Color color)
{
    sf::Vertex* outline = new sf::Vertex[5];
    outline[0] = sf::Vector2f(p0.x, p0.y),
    outline[1] = sf::Vector2f(p0.x+halfsize.x*2, p0.y),
    outline[2] = sf::Vector2f(p0.x+halfsize.x*2, p0.y+halfsize.y*2),
    outline[3] = sf::Vector2f(p0.x, p0.y+halfsize.y*2),
    outline[4] = sf::Vector2f(p0.x, p0.y);

    outline[0].color = color;
    outline[1].color = color;
    outline[2].color = color;
    outline[3].color = color;
    outline[4].color = color;

    return outline;
}