#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Texture& texture, sf::Vector2f position, float speed, float damage, float range);
    void update(sf::Time deltaTime);
    bool isAlive();
    void draw(sf::RenderWindow& window);
private:
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    float m_speed;
    float m_damage;
    float m_range;
    float m_distanceTravelled;
};

class Shooter {
public:
    Shooter(sf::Texture& texture, sf::Vector2f position, float speed, float fireRate);
    void update(sf::Time deltaTime);


private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed;
    float fireRate;
    float timeSinceLastShot;
    std::vector<Bullet> bullets;

    void shoot();
    void updateProjectiles(sf::Time deltaTime);
};