#pragma once

#define cell 32

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum Direction {
    None, Up, Down, Left, Right
};

enum State {
    Moving, Still, Attaking, Dead
};

class Entity;

class PathFinder {
public:
    PathFinder() = default;
    PathFinder(const int*, sf::Vector2i, std::map<int, int>);
    Direction pathfind(sf::Vector2i, sf::Vector2i);

    void initEntities(std::vector<Entity*>* e) { entities = e;};
    
    void info();
    bool isInit() { return init; };
    bool isAccesible(const Direction, const sf::Vector2i);

    std::vector<std::vector<int>>* getAcceses() { return &accesible; };
private:
    void makeCosts();
    const int* level;
    std::vector<std::vector<int>> accesible;
    std::vector<Entity*>* entities = nullptr;
    std::map<int, int> costs;
    sf::Vector2i size;
    bool init = false;
};

class Entity {
public:
    Entity(std::string, sf::Vector2i);
    inline void draw(sf::RenderWindow& window) { window.draw(sprite); };
    void initPatfind(const int*, sf::Vector2i, std::map<int, int>, std::vector<Entity*>*);
    void initAnimation(std::map<State, int> a) { animations = a; };

    void move(sf::Vector2i);
    void move(Direction);
    void update();

    sf::Vector2i getPosition() { return position; };
    Direction getDirection() { return direction; };
    State getState() { return state; };

    void setSpeed(float speed) { this->speed = speed; };
    void setState(State state) { this->state = state; };
    void setPosition(sf::Vector2i position);
protected:
    PathFinder pf;
    sf::Clock clock;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i position;
    std::map<State, int> animations;
    Direction direction = None;
    Direction lastDirection = None;
    State state = Still;
    float speed = (float)0.1;

    void animate(State, int);
    void move();
};

class Utils {
public:
    static sf::Vector2i makeDir(Direction);
    static sf::Vector2i gPos(sf::Vector2f);
    static void vecCout(sf::Vector2i, std::vector<std::vector<int>>);
};



class Enemy : public Entity {
public:
    Enemy(std::string str, sf::Vector2i pos) : Entity(str, pos) {};
    void initStats(int, int, float);
    bool doDamage(int);
private:
    int health = 1;
    int range = 10;
};

class Player : public Entity {
public:
    Player(std::string str, sf::Vector2i pos) : Entity(str, pos), spawn(pos) { speed = (float)0.2; animations = { {Moving, 2} }; };
    bool doDamage();

    int getLives() { return lives; };
    Direction getLastDirection() { return lastDirection; };

    void setSpawn(sf::Vector2i s) { spawn = s; };
private:
    sf::Vector2i spawn;
    int lives = 5;
    int damage = 1;
};