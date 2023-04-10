#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Entity.h"

class Enemy;
struct Level;
enum Direction;

class Bullet {
public:
    Bullet(sf::Vector2f pos, char dir, int dmg);
    Bullet::Bullet(sf::Vector2i pos, Direction dir, int dmg) : Bullet(sf::Vector2f(pos* cell), dir, dmg) {};
    sf::Vector2f update(float dt);
    sf::Vector2f Bullet::getPosition();
    char Bullet::getDirection();
    bool Bullet::checkCollisions(Level* level);

    inline bool operator==(const Bullet& c) {
        if (position == c.position && direction == c.direction)
            return true;
        return false;
    }

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    char direction;
    int damage = 1;
    sf::Texture texture;
};