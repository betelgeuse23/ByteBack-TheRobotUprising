#include "bullet.h"

Bullet::Bullet(sf::Vector2f pos, char dir, int dmg) : damage(dmg) {
	position = pos;
	if (dir == 'd' || dir == 'n') velocity = sf::Vector2f(0, 5.0);
	else if (dir == 'u') velocity = sf::Vector2f(0, -5.0);
	else if (dir == 'r') velocity = sf::Vector2f(5.0, 0);
	else if (dir == 'l') velocity = sf::Vector2f(-5.0, 0);
	direction = dir;
}

sf::Vector2f Bullet::update(float dt) {
	position += velocity * dt * (float)0.09;
	return position;
}

sf::Vector2f Bullet::getPosition() {
	return position;
}

char Bullet::getDirection() {
	return direction;
}

bool Bullet::checkCollisions(Level* level) {
	int tileBullet = std::roundf(position.x / 32) + std::roundf(position.y / 32) * 21;
	if (position.y < 0 && direction == 'u' ||
		position.y > 672 && (direction == 'd' || direction == 'n') ||
		position.x < 0 && direction == 'l' ||
		position.x > 672 && direction == 'r') return true;
	else if (level->map[tileBullet] == 5) return true;
	else if (level->map[tileBullet] == 2 || level->map[tileBullet] == 1) {
		level->map[tileBullet]--;
		return true;
	} else {
		int tileEnemy = level->player->getPosition().x + level->player->getPosition().y * 21;
		if (tileBullet == tileEnemy) {
			level->player->doDamage();
			return true;
		}
		if (!(level->enemies.empty())) {
			for (auto& enemy : level->enemies) {
				tileEnemy = enemy->getPosition().x + enemy->getPosition().y * 21;
				if (tileBullet == tileEnemy) {
					enemy->doDamage(1);
					return true;
				}
			}
		}
	}
	return false;
}