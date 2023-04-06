#include "main.h"

double frame = 0;

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Metal-Menace", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    sf::Clock clock;

    // персонаж
    sf::Image image;
    image.loadFromFile("images/robot2.png");
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite player;
    player.setTexture(texture);
    player.setTextureRect(sf::IntRect(0, 0, 31, 31));
    player.setPosition(0, 0);


    // карта
    const int level[] = {
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
        39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    };

    TileMap map;
    if (!map.load("images/test.png", sf::Vector2u(32, 32), level, 16, 16))
        return -1;


    // работа с окном
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 2800;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // Закрываем окно
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move(-1.0 * time, 0);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move(1.0 * time, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move(0, -1.0 * time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move(0, 1.0 * time);
        }
        frame += 0.005 * time;
        if (frame > 4) frame -= 4;
        player.setTextureRect(sf::IntRect(32 * int(frame), 0, 31, 31));

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(player);
        window.display();
    }

    return 0;
}