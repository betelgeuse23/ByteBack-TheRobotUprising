﻿#include "main.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Metal-Menace", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    sf::Clock clock;
    std::vector<Bullet> bullets;

    // персонаж
    sf::Image image;
    image.loadFromFile("images/character.png");
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite player;
    player.setTexture(texture);
    player.setTextureRect(sf::IntRect(0, 0, 31, 31));
    player.setPosition(0, 0);

    // карта
    const int level[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,
    };

    TileMap map;
    if (!map.load("images/tileset.png", sf::Vector2u(32, 32), level, 16, 16))
        return -1;

    bool isMoving = false;
    char dir = 'n';
    float delay;
    float move = 0;

    bool isShooting = false;
    float bulDelay = 0;

    // работа с окном
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 300;

        if (!(isMoving)) delay = 0;
        else delay += time;

        if (isShooting) bulDelay = 0;
        else bulDelay += time;

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

        // обработка нажатий клавиш
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !(isMoving)) {
            isMoving = true;
            dir = 'l';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !(isMoving)) {
            isMoving = true;
            dir = 'r';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !(isMoving)) {
            isMoving = true;
            dir = 'u';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !(isMoving)) {
            isMoving = true;
            dir = 'd';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (bulDelay > 2000)
            {
                isShooting = true;
                Bullet bullet(player.getPosition(), dir, "images/character.png");
                bullets.push_back(bullet);
            }
            else isShooting = false;
        }

        if (isMoving && dir == 'n' && delay/1000 > 0.5) isMoving = false;

        if (isMoving && move < 32 && dir != 'n') {
            move += 0.1 * time;
            float k = move > 32 ? move - 32 : 0;
            if (dir == 'u') {
                player.move(0, -0.1 * time + k);
            } else if (dir == 'd') {
                player.move(0, 0.1 * time - k);
            } else if (dir == 'r') {
                player.move(0.1 * time - k, 0);
            } else if (dir == 'l') {
                player.move(-0.1 * time + k, 0);
            }
            player.setTextureRect(sf::IntRect(32 * (int(move/8) % 4), 0, 31, 31));
        }
        else {
            move = 0;
            dir = 'n';
            player.setTextureRect(sf::IntRect(0, 0, 31, 31));
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(player);
        for (auto& elem : bullets) { 
            elem.getSprite().setPosition(elem.update(0.025 * time));
            window.draw(elem.getSprite());
        }
        window.display();
    }

    return 0;
}