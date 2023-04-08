#include "bullet.h"

Bullet::Bullet(sf::Vector2f pos, char dir) {
	position = pos;
	if (dir == 'd' || dir == 'n') velocity = sf::Vector2f(0, 5.0);
	else if (dir == 'u') velocity = sf::Vector2f(0, -5.0);
	else if (dir == 'r') velocity = sf::Vector2f(5.0, 0);
	else if (dir == 'l') velocity = sf::Vector2f(-5.0, 0);
	direction = dir;
}

sf::Vector2f Bullet::update(float dt) {
	position += velocity * dt;
	return position;
}

sf::Vector2f Bullet::getPosition() {
	return position;
}

char Bullet::getDirection() {
	return direction;
}

//bool checkCollisionsWithEnemy(std::vector<Enemy>& enemies) {
//
//}

bool Bullet::checkCollisionsWithObstacle(int* obstacles) {
	int tile = int(position.x / 32 + 0.5) + int(position.y / 32 + 0.5) * 21;
	if (obstacles[tile] == 5) return true;
	else if (obstacles[tile] == 2) {
		obstacles[tile] = 1;
		return true;
	}
	else if (obstacles[tile] == 1) {
		obstacles[tile] = 0;
		return true;
	}
	return false;
}