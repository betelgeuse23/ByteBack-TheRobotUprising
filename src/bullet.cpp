#include "bullet.h"

Bullet::Bullet(sf::Vector2f pos, char dir, std::string img) {
	position = pos;
	if (dir == 'd' || dir == 'n') velocity = sf::Vector2f(0, 5.0);
	else if (dir == 'u') velocity = sf::Vector2f(0, -5.0);
	else if (dir == 'r') velocity = sf::Vector2f(5.0, 0);
	else if (dir == 'l') velocity = sf::Vector2f(-5.0, 0);
	direction = dir;
	sf::Image image;
	image.loadFromFile(img);
	sf::Texture texture;
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 31, 31));
}

sf::Vector2f Bullet::update(float dt) {
	position += velocity * dt;
	return position;
}

sf::Sprite& Bullet::getSprite() {
	return sprite;
}