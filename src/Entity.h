#pragma once

#define cell 32

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include "bullet.h"

enum Direction {
    None = 'n', Up = 'u', Down = 'd', Left = 'l', Right = 'r'
};

enum State {
    Moving, Still, Attaking, Dead
};

class Entity;
class Enemy;
class Player;
class Bonus;
class Bullet;

struct Level {
    int* map = nullptr;
    sf::Vector2i size;
    sf::Vector2i base;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    std::vector<Bonus*> bonuses;
    Player* player = nullptr;
};

class PathFinder {
public:
    PathFinder() = default;
    PathFinder(Level*, Entity*, std::map<int, int>);
    Direction pathfind(sf::Vector2i);
    
    void info();
    bool isInit() { return init; };
    bool isAccesible(const Direction, const sf::Vector2i);
    bool doTrace(const Direction, const sf::Vector2i, int);

    Level* getLevel() { return level; };
    std::vector<std::vector<int>>* getAcceses() { return &accesible; };
private:
    Level* level;
    Entity* that;
    std::map<int, int> costs;
    std::vector<std::vector<int>> accesible;
    bool init = false;

    void makeCosts(bool);
};

class Entity {
public:
    Entity(std::string, sf::Vector2i);
    inline void draw(sf::RenderWindow& window) { window.draw(sprite); };
    void initPatfind(Level*, std::map<int, int>);
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
    Direction lastDirection = Down;
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
    Enemy(std::string str, sf::Vector2i pos) : Entity(str, pos) { animations = { {Moving, 4} }; };
    void makeTarget() { target = pf.getLevel()->base; }
    void initStats(int, int, float);
    bool doDamage(int);
    void shoot(Level*, Direction);
    void update();
    bool isCharged();
private:
    sf::Vector2i target;
    sf::Clock fire;
    int health = 1;
    int range = 10;
};

class Player : public Entity {
public:
    Player(std::string str, sf::Vector2i pos) : Entity(str, pos), spawn(pos) { speed = (float)0.2; animations = { {Moving, 2} }; };
    bool doDamage(int);

    int getLives() { return lives; };
    Direction getLastDirection() { return lastDirection; };
    int getDamage() { return damage; };
    
    void update();
    void shoot(Level*);

    void bonusSpeed();
    void bonusLives();
    void bonusDamage();

    bool isCharged();
    void setSpawn(sf::Vector2i s) { spawn = s; };
private:
    sf::Clock effect;
    sf::Clock fire;
    bool charge;
    sf::Vector2i spawn;
    int lives = 5;
    int damage = 1;
};
