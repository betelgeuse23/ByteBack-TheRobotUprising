#pragma once

#define cell 32

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include "bullet.h"
#include "Bonus.h"

enum Direction {
    None, Up, Down, Left, Right
};

enum State {
    Moving, Still, Attaking, Dead
};

class Entity;
class Enemy;
class Player;
class Bonus;
class Bullet;
class Spawner;
enum Effects;

class Mpl {
public:
    void playShoot();
    void playHit();
    void playGet();
    void playLoose();
    void playWin();
    void off() { offed = true; };
private:
    bool offed = false;
    sf::SoundBuffer sb;
    sf::Sound s;
};

struct Level {
    Mpl pl;
    sf::Vector2i size;
    sf::Vector2i base;
    std::vector<Enemy*> enemies;
    std::vector<Bonus*> bonuses;
    std::vector<Bullet*> bullets;
    std::vector<Player*> players;
    std::vector<std::vector<int>> map;
    void clear() {
        for (auto i : enemies) delete i;
        for (auto i : bonuses) delete i;
        for (auto i : bullets) delete i;
        for (auto i : players) delete i;
    }
    void translate(int* mapT) {
        map = std::vector<std::vector<int>>(size.x, std::vector<int>(size.y));
        for (int i = 0, t = 0; i < size.x; i++) for (int j = 0; j < size.y; j++, t++) {
            map[j][i] = mapT[t]<9?mapT[t]:0;
            if (mapT[t] == 6) base = sf::Vector2i(j, i);
        }
    }
};

class PathFinder {
public:
    PathFinder() = default;
    PathFinder(Level* level, std::map<int, int> costs)
        : level(level), accesible(std::vector<std::vector<int>>(level->size.x, std::vector<int>(level->size.y))), costs(costs), matrix(std::vector<std::vector<int>>(level->size.x, std::vector<int>(level->size.y))) {};

    Direction pathfind(sf::Vector2i, sf::Vector2i);
    bool isAccesible(const Direction, const sf::Vector2i);
    bool doTrace(const Direction, const sf::Vector2i, int, bool);

    void info();
    Level* getLevel() { return level; };
private:
    Level* level;
    std::map<int, int> costs;
    std::vector<std::vector<int>> accesible;
    std::vector<std::vector<int>> matrix;
    void makeCosts(sf::Vector2i, bool);
    void makeMatrix();
    void makeAccess();
};

class Entity {
public:
    Entity(std::string, sf::Vector2i);
    inline void draw(sf::RenderWindow& window) { window.draw(sprite); };
    void initPatfind(PathFinder* pf) { this->pf = pf; };
    void initAnimation(std::map<State, int> a) { animations = a; };

    void move(sf::Vector2i);
    void move(Direction);
    void update();

    sf::Vector2f getSpritePosition() { return sprite.getPosition(); };
    sf::Vector2i getPosition() { return position; };
    Direction getDirection() { return direction; };
    State getState() { return state; };

    void setSpeed(float speed) { this->speed = speed; };
    void setState(State state) { this->state = state; };
    void setPosition(sf::Vector2i position);
protected:
    PathFinder* pf = nullptr;
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
    Enemy(std::string str, sf::Vector2i pos) : Entity(str, pos) {};
    void makeTarget() { target = pf->getLevel()->base; }
    void initStats(int, int, int, float);
    bool doDamage(int);
    void shoot(Level*, Direction);
    void update();
    bool isCharged();
private:
    sf::Vector2i target;
    sf::Clock fire;
    int health = 1;
    int damage = 1;
    int range = 10;
};

class Player : public Entity {
public:
    Player(std::string str, sf::Vector2i pos, int col) : Entity(str, pos), spawn(pos), color(col) { speed = (float)0.2; animations = { {Moving, 2} }; };
    bool doDamage(int);

    int getLives() { return lives; };
    Direction getLastDirection() { return lastDirection; };
    int getDamage() { return damage; };
    
    void update();
    void shoot(Level*);

    void affect(Effects);
    Effects getEffect() { return effect; };
    int getColor() { return color; };
    void setSpawn(sf::Vector2i s) { spawn = s; };
private:
    int color;
    sf::Clock bonus;
    sf::Clock fire;
    bool charge;
    sf::Vector2i spawn;
    int lives = 5;
    int damage = 1;
    int rate = 1000;
    Effects effect = Effects(6);
    sf::SoundBuffer sbS, sbD, sbB;
    sf::Sound sS, sD, sB;

    bool isCharged();
};


class Spawner {
public:
    Spawner(Level* level) : level(level) {
        pfP = PathFinder(level, { {0, 1}, {4, 1}, {6, 1} });
        pfE = PathFinder(level, { {0, 1}, {6, 1}, {1, 2}, { 2, 3 } });
        pfEA = PathFinder(level, { {0, 1}, {6, 1}, {1, 2}, { 2, 3 }, { 3, 1 }, { 4, 1 } });
        pfEB = PathFinder(level, { {0, 1}, {6, 1}, {1, 2}, { 2, 3 }, { 3, 1 } });
    };
    void spawn(int*);
private:
    Level* level;
    PathFinder pfP, pfE, pfEA, pfEB;

    void spawnPlayer(int, sf::Vector2i);
    void spawnRobot1(sf::Vector2i);
    void spawnRobot2(sf::Vector2i);
    void spawnRobot3(sf::Vector2i);
    void spawnRobot4(sf::Vector2i);
    void spawnRobot5(sf::Vector2i);
};