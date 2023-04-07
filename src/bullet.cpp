#include "bullet.h"

Bullet::Bullet(sf::Texture& texture, sf::Vector2f position, float speed, float damage, float range)
    : m_speed(speed), m_damage(damage), m_range(range), m_distanceTravelled(0.0f)
{
    // Initialize sprite and set position
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    m_sprite.setPosition(position);

    // Calculate velocity based on speed and direction
    sf::Vector2f direction = /* calculate direction towards target */;
    float distance = /* calculate distance to target */;
    m_velocity = direction * (m_speed / distance);
}

void Bullet::update(sf::Time deltaTime) {
    sprite.move(velocity * deltaTime.asSeconds());
    traveled += speed * deltaTime.asSeconds();
}

bool Bullet::isAlive() {
    return traveled < range;
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Shooter::update(sf::Time deltaTime) {
    fireTimer += deltaTime.asSeconds();
}

void Shooter::shoot() {
    if (fireTimer >= fireRate) {
        // Create a new Bullet
        Bullet p(texture, sprite.getPosition(), speed, damage, range);
        // Set the velocity of the Bullet
        sf::Vector2f direction = // Determine the direction of the Bullet
            p.setVelocity(direction);
        // Add the Bullet to a list or vector of Bullets
        bullets.push_back(p);
        // Reset the fire timer
        fireTimer = 0;
    }
}

void Shooter::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}