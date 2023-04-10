#include "menu.h"

Menu::Menu(sf::RenderWindow& window, sf::String name[])
	:menuWindow(window)
{  
	font.loadFromFile("fonts/arial.ttf");
	mainMenu = new sf::Text[5];
	for (int i = 0, ypos = 192; i < 5; i++, ypos += 64) setInitText(mainMenu[i], name[i], 96, ypos);
	mainMenuSelected = 0; 
}

void Menu::setInitText(sf::Text& text, sf::String str, float xpos, float ypos)
{
	text.setFont(font);
	text.setFillColor(menu_text_color);
	text.setString(str);                
	text.setCharacterSize(30);
	text.setLetterSpacing(2);
	text.setPosition(xpos, ypos);
}

void Menu::draw()
{ 	
	for (int i = 0; i < 5; i++) menuWindow.draw(mainMenu[i]);
}

int Menu::getSelectedMenuNumber()
{
	return mainMenuSelected;
}

int Menu::MoveUp()
{
	mainMenuSelected--;
	if (mainMenuSelected == -1) {
		mainMenuSelected = 4;
	}
	return mainMenuSelected;
}

int Menu::MoveDown()
{
	mainMenuSelected++;
	if (mainMenuSelected == 5) {
		mainMenuSelected = 0;
	}
	return mainMenuSelected;
}