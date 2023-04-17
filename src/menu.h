#pragma once
#define WIDTH 21
#include "TileMap.h"
#include "bullet.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Menu
{
public:
	sf::Font font;
	sf::Color menu_text_color = sf::Color::White;
	Menu(sf::RenderWindow& window, sf::String name[]);
	~Menu()
	{
		delete[] mainMenu;
	}
	void setInitText(sf::Text& text, sf::String str, float xpos, float ypos);
	void draw();   
	int MoveUp();
	int MoveDown(); 
	int getSelectedMenuNumber();

	void startGame(sf::RenderWindow& window, sf::Clock& clock);
	void Options(sf::RenderWindow& window);
	void Tutorial(sf::RenderWindow& window);
	void savingScreen(sf::RenderWindow& window);

private:
	int mainMenuSelected;
	sf::Text* mainMenu;
	sf::RenderWindow& menuWindow;
};