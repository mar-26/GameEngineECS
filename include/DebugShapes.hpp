#ifndef DEBUGSHAPES_HPP
#define DEBUGSHAPES_HPP

#include "Vector.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>

sf::Vertex* boundingCircle(float radius, const Vector& position, sf::Color color);
sf::Vertex* debugLine(const Vector& p0, const Vector& p1, sf::Color color);

#endif