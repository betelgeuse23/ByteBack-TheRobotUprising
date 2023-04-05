#include "game.h"

#include <SFML/Graphics.hpp>


int main()
{
	// Создаем окно
	sf::RenderWindow window(sf::VideoMode(600, 600), "game", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
	window.setFramerateLimit(60);

	const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
	};

	/*TileMap map;
	if (!map.load("./image/tileset.png", sf::Vector2u(32, 32), level, 16, 8))
		return -1;*/

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

		window.clear(sf::Color::Black);
		//window.draw(map);
		window.display();
	}

	return 0;
}
