#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f pos, char dir);
    sf::Vector2f update(float dt);
    //bool checkCollisionsWithEnemy(std::vector<Enemy>& enemies);
    //bool checkCollisionsWithObstacle(std::vector<Obstacle>& obstacles);

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    char direction;
    sf::Texture texture;
};