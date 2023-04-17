#pragma once
#define WIDTH 21
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "bullet.h"
#include "Entity.h"
#include "menu.h"

class Game {
public:
	void startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu);
private:
	int level;
};