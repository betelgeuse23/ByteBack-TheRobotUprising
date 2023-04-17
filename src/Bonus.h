#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

struct Level;

enum Effects {
	Health, Speed, Rate, Shield, Damage, Spare
};

class Bonus {
public:
	Bonus(Effects, sf::Vector2i);
	inline void draw(sf::RenderWindow& window) { window.draw(sprite); };
	bool checkCollisions(Level*);
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2i position;
	Effects effect;
};