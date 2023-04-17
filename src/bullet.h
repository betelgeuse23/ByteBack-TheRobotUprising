#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Entity.h"
#include "Main.h"

class Enemy;
struct Level;
enum Direction;

class Bullet {
public:
    Bullet(sf::Vector2f pos, Direction dir, int dmg, bool pl) : damage(dmg), players(pl), position(pos), direction(dir) {};
    Bullet::Bullet(sf::Vector2i pos, Direction dir, int dmg, bool pl) : Bullet(sf::Vector2f(pos* cell), dir, dmg, pl) {};

    sf::Vector2f update(float dt);
    sf::Vector2f Bullet::getPosition();
    bool Bullet::checkCollisions(Level* level);

private:
    sf::Vector2f position;
    Direction direction;
    int damage = 1;
    bool players = false;
};