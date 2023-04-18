#pragma once
#define WIDTH 21
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TileMap.h"
#include "bullet.h"
#include "Entity.h"
#include "Maps.h"
#include "menu.h"
#include <fstream>
#include <sstream>

class Menu;
struct Options;

class Game {
public:
	void startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu, Options& opts);
	int getLevel() { return level; }
	void setLevel(int lvl) { level = lvl; }
private:
	int level = 1;
};