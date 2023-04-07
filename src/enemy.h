class Enemy {
public:
    Enemy(sf::Texture& texture, sf::Vector2f position, float speed);

    void update(sf::Time deltaTime, const std::vector<int>& level, int playerX, int playerY);
    void draw(sf::RenderWindow& window);
    void findShortestPath(Node* startNode, Node* targetNode, const int* obstacles);

private:
    sf::Sprite sprite;
    sf::Vector2f position;
};

class Node {
public:
    Node(int x, int y, bool isObstacle);
    int x, y; // position on the board
    bool isObstacle; // true if this node is an obstacle
    int distanceFromStart; // distance from starting point
    bool visited; // true if this node has been visited in the pathfinding algorithm
    Node* parent; // the previous node in the shortest path
};
