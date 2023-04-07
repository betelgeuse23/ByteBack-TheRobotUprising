#pragma once

#include <SFML/Graphics.hpp>
#include <queue>
#include <cmath>
#include <iostream>
#include <vector>

const int WIDTH = 600;
const int HEIGHT = 600;
const int CELL_SIZE = 32;
const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;
const int INF = INT_MAX;

class Cell {
public:
    int x, y;
    int gCost, hCost, fCost;
    bool obstacle, visited;
    Cell* parent;

    Cell();
    Cell(int x, int y, bool obstacle);
    int getDistance(Cell* other);
};

struct CompareCells {
    bool operator()(const Cell* lhs, const Cell* rhs) const {
        return lhs->fCost > rhs->fCost;
    }
};

class Enemy {
public:
    sf::RectangleShape shape;
    std::vector<Cell*> path;
    int pathIndex;
    bool isActive;

    Enemy(sf::Vector2f position, sf::Color color);
    void update();
    void draw(sf::RenderWindow& window);
    void findPath(Cell* start, Cell* goal, std::vector<Cell*>& grid);
    std::vector<Cell*> getPath(Cell* start, Cell* goal);

private:
    void addNeighbors(Cell* current, Cell* goal, std::priority_queue<Cell*, std::vector<Cell*>, CompareCells>& openList, std::vector<Cell*>&);
};

