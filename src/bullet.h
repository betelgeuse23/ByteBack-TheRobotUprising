#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
public:
    Bullet(sf::Vector2f pos, char dir);
    sf::Vector2f update(float dt);
    sf::Vector2f Bullet::getPosition();
    char Bullet::getDirection();
    //bool checkCollisionsWithEnemy(std::vector<Enemy>& enemies);
    bool checkCollisionsWithObstacle(const int* obstacles);

    inline bool operator==(const Bullet& c) {
        if (position == c.position && direction == c.direction)
            return true;
        return false;
    }

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    char direction;
    sf::Texture texture;
};