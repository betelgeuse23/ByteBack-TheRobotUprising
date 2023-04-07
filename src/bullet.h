#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f pos, char dir, char* img);
    sf::Vector2f update(float dt);
    sf::Sprite& getSprite();
    //bool checkCollisionsWithEnemy(std::vector<Enemy>& enemies);
    //bool checkCollisionsWithObstacle(std::vector<Obstacle>& obstacles);

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    char direction;
    sf::Sprite sprite;
};