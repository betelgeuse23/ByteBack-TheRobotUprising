#include "Bonus.h"

Bonus::Bonus(Effects ef, sf::Vector2i pos) : position(pos), effect(ef) {
	texture.loadFromFile("images/bonuses.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(32 * effect, 0, 31, 31));
	sprite.setPosition(sf::Vector2f(position * cell));
}

bool Bonus::checkCollisions(Level* level) {
	if (!(level->players.empty()))
		for (auto& p : level->players)
			if (p->getPosition() == position) {
				p->affect(effect);
				return true;
			}
	return false;
}