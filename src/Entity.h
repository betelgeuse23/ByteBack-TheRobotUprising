#pragma once

#define cell 32

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum Direction {
    None, Up, Down, Left, Right
};

enum State {
    Moving, Still, Shooting, Dead
};


class PathFinder {
public:
    PathFinder() {};
    PathFinder(const int*, sf::Vector2i, std::map<int, int>);
    Direction pathfind(sf::Vector2i, sf::Vector2i);
    std::vector<std::vector<int>>* getAcceses() { return &accesible; };
    void info();
    bool isInit() { return init; };
    bool isAccesible(const Direction, const sf::Vector2i);
private:
    void makeCosts();
    const int* level;
    std::vector<std::vector<int>> accesible;
    std::map<int, int> costs;
    sf::Vector2i size;
    bool init = false;
};

class Entity {
public:
    Entity(std::string, sf::Vector2i);
    inline void draw(sf::RenderWindow& window) { window.draw(sprite); };
    void initPatfind(const int*, sf::Vector2i, std::map<int, int>);

    void move(sf::Vector2i);
    void move(Direction);

    sf::Vector2i getPosition() { return position; };
    State getState() { return state; };

    void setSpeed(float speed) { this->speed = speed; };
    void setState(State state) { this->state = state; };
private:
    PathFinder pf;
    sf::Clock clock;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i position;
    Direction direction = None;
    State state = Still;
    float speed = 0.1;

    void animate(State, int, int);
};

class Utils {
public:
    static sf::Vector2i makeDir(Direction);
    static sf::Vector2i gPos(sf::Vector2f);
    static void vecCout(sf::Vector2i, std::vector<std::vector<int>>);
};