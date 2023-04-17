#include "bullet.h"

Bullet::Bullet(sf::Vector2f pos, char dir, int dmg, bool pl) : damage(dmg), players(pl) {
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
	sf::Vector2i globalPos =  Utils::gPos(position) + Utils::makeDir(Direction(direction));
	int tileBullet = globalPos.x + globalPos.y * level->size.x;
	if (globalPos.y < 0 && direction == Direction::Up || globalPos.y >= level->size.y && direction == Direction::Down || globalPos.x < 0 && direction == Direction::Left || globalPos.x >= level->size.x && direction == Direction::Right)
		return true;

	int tile = level->map[tileBullet];
	if (tile == 1 || tile == 2 || tile == 5) {
		if(tile != 5) level->map[tileBullet]--;
		return true;
	}

	if (!(players) && level->player->getPosition() == globalPos) {
		level->player->doDamage(1);
		return true;
	}

	if (players && !(level->enemies.empty()))
		for (auto& enemy : level->enemies)
			if (enemy->getPosition() == globalPos) {
				enemy->doDamage(1);
				return true;
			}
	return false;
}