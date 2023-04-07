#include "enemy.h"

Enemy::Enemy(sf::Texture& texture, sf::Vector2f position, float speed)
{
    // Set the texture and initial position of the enemy
    m_sprite.setTexture(texture);
    m_sprite.setPosition(position);

    // Set the speed of the enemy
    m_speed = speed;

    // Set the initial direction of the enemy to Up
    m_direction = Direction::Up;

    // Set the initial state of the enemy to Alive
    m_state = State::Alive;

    // Set the initial health of the enemy
    m_health = 10;

    // Set the initial damage of the enemy's bullets
    m_bulletDamage = 10;
}

void Enemy::update(sf::Time deltaTime, const std::vector<int>& level, int playerX, int playerY) {
    // Move the enemy towards the player
    std::vector<sf::Vector2i> path = findShortestPath(level, getPosition(), sf::Vector2i(playerX, playerY));
    if (!path.empty()) {
        // The first element in the path is the enemy's next target position
        sf::Vector2i nextPosition = path[0];

        // Calculate the direction of movement
        sf::Vector2i direction = nextPosition - getPosition();
        if (direction.x != 0) {
            direction.x /= std::abs(direction.x);
        }
        if (direction.y != 0) {
            direction.y /= std::abs(direction.y);
        }

        // Calculate the new position
        sf::Vector2f position = getPosition();
        position.x += mSpeed * direction.x * deltaTime.asSeconds();
        position.y += mSpeed * direction.y * deltaTime.asSeconds();

        // Update the enemy's position
        setPosition(position);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}

void Enemy::findShortestPath(Node* startNode, Node* targetNode, const int* obstacles) {
    // create a 2D array of nodes to represent the game board
    Node* board[16][16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            bool isObstacle = obstacles[i * 16 + j] == 1; // check if this tile is an obstacle
            board[i][j] = new Node(i, j, isObstacle); // create a new node for this tile
        }
    }

    // initialize the start node
    startNode->distanceFromStart = 0;
    startNode->visited = true;

    // create a queue of nodes to visit in the pathfinding algorithm
    std::queue<Node*> nodeQueue;
    nodeQueue.push(startNode);

    // continue pathfinding until we reach the target node
    while (!nodeQueue.empty()) {
        Node* currentNode = nodeQueue.front();
        nodeQueue.pop();

        // check if we've reached the target node
        if (currentNode == targetNode) {
            break;
        }

        // check each neighbor of the current node
        std::vector<Node*> neighbors;
        if (currentNode->x > 0) {
            neighbors.push_back(board[currentNode->x - 1][currentNode->y]);
        }
        if (currentNode->x < 15) {
            neighbors.push_back(board[currentNode->x + 1][currentNode->y]);
        }
        if (currentNode->y > 0) {
            neighbors.push_back(board[currentNode->x][currentNode->y - 1]);
        }
        if (currentNode->y < 15) {
            neighbors.push_back(board[currentNode->x][currentNode->y + 1]);
        }

        // update the distance and parent node of each neighbor
        for (Node* neighbor : neighbors) {
            if (!neighbor->visited && !neighbor->isObstacle) {
                neighbor->visited = true;
                neighbor->parent = currentNode;
                neighbor->distanceFromStart = currentNode->distanceFromStart + 1;
                nodeQueue.push(neighbor);
            }
        }
    }

    // once we've found the shortest path, you can navigate your enemy along it using the `parent` property of each node
}

Node::Node(int x, int y, bool isObstacle) :
    x(x),
    y(y),
    isObstacle(isObstacle),
    distanceFromStart(INT_MAX),
    visited(false),
    parent(nullptr)
{
}
