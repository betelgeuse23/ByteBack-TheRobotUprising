#include "enemy.h"

/*Cell::Cell() {
    x = 0;
    y = 0;
    gCost = INF;
    hCost = INF;
    fCost = INF;
    obstacle = false;
    visited = false;
    parent = nullptr;
}

Cell::Cell(int x, int y, bool obstacle) {
    this->x = x;
    this->y = y;
    gCost = INF;
    hCost = INF;
    fCost = INF;
    this->obstacle = obstacle;
    visited = false;
    parent = nullptr;
}

int Cell::getDistance(Cell* other) {
    int dx = abs(x - other->x);
    int dy = abs(y - other->y);
    return (dx + dy);
}

Enemy::Enemy(sf::Vector2f position, sf::Color color) {
    shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    shape.setPosition(position);
    shape.setFillColor(color);
    pathIndex = 0;
    isActive = true;
}

void Enemy::update() {
    if (isActive && pathIndex < path.size()) {
        Cell* nextCell = path[pathIndex];
        sf::Vector2f nextPosition = sf::Vector2f(nextCell->x * CELL_SIZE, nextCell->y * CELL_SIZE);
        if (shape.getPosition() == nextPosition) {
            pathIndex++;
        }
        else {
            sf::Vector2f direction = nextPosition - shape.getPosition();
            float distance = std::sqrt(std::pow(direction.x, 2) + std::pow(direction.y, 2));
            if (distance > 0.0f) {
                direction /= distance;
                shape.move(direction * 2.0f);
            }
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Enemy::findPath(Cell* start, Cell* goal, std::vector<Cell*>& grid) {
    path.clear();
    pathIndex = 0;

    std::priority_queue<Cell*, std::vector<Cell*>, CompareCells> openList;
    std::vector<Cell*> closedList;

    start->gCost = 0;
    start->hCost = start->getDistance(goal);
    start->fCost = start->hCost;
    openList.push(start);

    while (!openList.empty()) {
        Cell* current = openList.top();
        openList.pop();
        if (current == goal) {
            path = getPath(start, goal);
            return;
        }
        current->visited = true;
        closedList.push_back(current);
        addNeighbors(current, goal, openList, grid);
    }
}

std::vector<Cell*> Enemy::getPath(Cell* start, Cell* goal) {
    std::vector<Cell*> path;
    Cell* current = goal;
    while (current != start) {
        path.insert(path.begin(), current);
        current = current->parent;
    }
    path.insert(path.begin(), start);
    return path;
}

void addNeighbors(Cell* current, Cell* goal, std::priority_queue<Cell*, std::vector<Cell*>, CompareCells>& openList, std::vector<Cell*>& visited) {
    std::vector<Cell*> neighbors = getNeighbors(current);
    for (Cell* neighbor : neighbors) {
        if (std::find(visited.begin(), visited.end(), neighbor) == visited.end() && !neighbor->isBlocked) {
            float newG = current->gCost + std::sqrt(std::pow(neighbor->x - current->x, 2) + std::pow(neighbor->y - current->y, 2));
            if (newG < neighbor->gCost) {
                neighbor->gCost = newG;
                neighbor->hCost = std::sqrt(std::pow(goal->x - neighbor->x, 2) + std::pow(goal->y - neighbor->y, 2));
                neighbor->fCost = neighbor->g + neighbor->hCost;
                neighbor->parent = current;
                openList.push(neighbor);
            }
        }
    }
}*/
