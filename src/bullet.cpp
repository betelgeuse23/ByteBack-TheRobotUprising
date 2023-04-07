#include "bullet.h"

Bullet::Bullet(sf::Vector2f pos, char dir) {
	position = pos;
	if (dir == 'd') velocity = sf::Vector2f(0, 5.0);
	else if (dir == 'u') velocity = sf::Vector2f(0, -5.0);
	else if (dir == 'r') velocity = sf::Vector2f(5.0, 0);
	else if (dir == 'l') velocity = sf::Vector2f(-5.0, 0);
	direction = dir;
}

sf::Vector2f Bullet::update(float dt) {
	position += velocity * dt;
	return position;
}