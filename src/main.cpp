// game.cpp: определяет точку входа для приложения.
//

#include "game.h"
#include <SFML/Graphics.hpp>


int main()
{
	// Создаем окно
	sf::RenderWindow window(sf::VideoMode(600, 600), "game", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
	window.setFramerateLimit(60);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				// Закрываем окно
				if (event.key.code == sf::Keyboard::Escape) window.close();
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}
