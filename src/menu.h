#pragma once
#define WIDTH 21
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "game.h"
#include <fstream>
#include <sstream>
#include <iostream>

class Menu
{
public:
	sf::Font font;
	sf::Color menu_text_color = sf::Color::White;
	sf::Color main_text_color = sf::Color::Yellow;
	Menu(sf::RenderWindow& window, sf::String name[], int itm, int xpos, int ypos, int sz, int dstn);
	~Menu()
	{
		delete[] mainMenu;
	}
	void setInitText(sf::Text& text, sf::String str, float xpos, float ypos, bool first);
	void draw();   
	int MoveUp();
	int MoveDown(); 
	int getSelectedMenuNumber();

	void Options(sf::RenderWindow& window);
	void Tutorial(sf::RenderWindow& window);
	bool savingScreen(sf::RenderWindow& window);
	void winScreen(sf::RenderWindow& window);
	void loseScreen(sf::RenderWindow& window);
	void chooseScreen(sf::RenderWindow& window);
	void createScreen(sf::RenderWindow& window);
	void levelScreen(sf::RenderWindow& window, sf::Clock& clock);

private:
	int mainMenuSelected;
	int items;
	int yposition;
	int xposition;
	int size;
	int distance;
	sf::Text* mainMenu;
	sf::RenderWindow& menuWindow;
};