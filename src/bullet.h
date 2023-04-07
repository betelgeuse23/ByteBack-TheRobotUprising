class Bullet {
public:
    Bullet(sf::Texture& texture, sf::Vector2f position, float speed, float damage, float range);
    void update(sf::Time deltaTime);
    bool isAlive();
    void draw(sf::RenderWindow& window);
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float damage;
    float range;
    float traveled;
};

class Shooter {
public:
    Shooter(sf::Texture& texture, sf::Vector2f position, float speed, float fireRate);
    void update(sf::Time deltaTime);
    void shoot();
    void draw(sf::RenderWindow& window);
private:
    sf::Sprite sprite;
    float speed;
    float fireRate;
    float fireTimer;
};