#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable, public sf::Transformable   {
public:
    bool load(const std::string&, sf::Vector2u, const int*, unsigned int, unsigned int);
    bool load(const std::string&, sf::Vector2u, const std::vector<std::vector<int>>, unsigned int, unsigned int);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};