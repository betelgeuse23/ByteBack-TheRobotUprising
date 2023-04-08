#pragma once

#define cell 32

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <string>

void log_i(std::string s);

enum Direction {
    None, Up, Down, Left, Right
};

enum State {
    Still, Moving, Shooting, Dead
};

class Entity {
public:
    Entity(std::string texture, sf::Vector2f position);
    void draw(sf::RenderWindow& window);
    void animate(int ch, int sec);
    void move(Direction dir);
    void move();
    sf::Vector2f pos() { return position; };
    void info();
private:
    sf::Clock clock;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    Direction direction = None;
    State state = Still;
    float speed = 0.1;
};

Direction pathfind(const int* level, sf::Vector2f size, sf::Vector2f pos, sf::Vector2f dest);

sf::Vector2f gPos(sf::Vector2f pos); 