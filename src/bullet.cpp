#include "bullet.h"

sf::Vector2f Bullet::update(float dt) {
	position += dt * (float)0.4 * sf::Vector2f(Utils::makeDir(direction));
	return position;
}

sf::Vector2f Bullet::getPosition() {
	return position;
}

bool Bullet::checkCollisions(Level* level) {
	sf::Vector2i globalPos =  Utils::gPos(position);
	if (globalPos.y < 0 && direction == Direction::Up || globalPos.y >= level->size.y && direction == Direction::Down || globalPos.x < 0 && direction == Direction::Left || globalPos.x >= level->size.x && direction == Direction::Right)
		return true;

	int tile = level->map[globalPos.x][globalPos.y];
	if (tile == 1 || tile == 2 || tile == 5) {
		if (tile != 5) {
			level->map[globalPos.x][globalPos.y]--;
			if (damage > 1 && tile == 2) level->map[globalPos.x][globalPos.y]--;
			level->pl.playHit();
		}
		return true;
	}

	if (!(players) && !(level->players.empty()))
		for (auto& p : level->players)
			if (Utils::gPos(p->getSpritePosition()) == globalPos) {
				p->doDamage(damage);
				level->pl.playHit();
				return true;
			}

	if (players && !(level->enemies.empty()))
		for (auto& e : level->enemies)
			if (Utils::gPos(e->getSpritePosition()) == globalPos) {
				e->doDamage(damage);
				level->pl.playHit();
				return true;
			}

	return false;
}