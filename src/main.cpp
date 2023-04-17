﻿#include "main.h"

int main() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    sf::Clock clock;
    float delay = 0;
    int gameLevel = 0;

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    // текст
    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titulFirst("Byte Back:", fontPixel, 70);
    titulFirst.setLetterSpacing(2);
    titulFirst.setFillColor(sf::Color::White);
    titulFirst.setPosition(20, 23);
    sf::Text titulSecond("the robot uprising", fontArial, 30);
    titulSecond.setLetterSpacing(2);
    titulSecond.setFillColor(sf::Color::White);
    titulSecond.setPosition(416, 96);
    sf::Text point(">", fontPixel, 35);
    point.setFillColor(sf::Color::White);


    sf::String name_menu[]{ L"START", L"CONTINUE", L"OPTIONS", L"TUTORIAL", L"QUIT" };
    Menu menu(window, name_menu);

    while (window.isOpen()) {
        sf::Event event;

        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        delay += time;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.key.code == sf::Keyboard::Up) { 
                if (delay > 300)
                {
                    menu.MoveUp();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::Down) { 
                if (delay > 300)
                {
                    menu.MoveDown();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::Return)
            {
                switch (menu.getSelectedMenuNumber())
                {
                case 0:menu.startGame(window, clock);   break;
                case 2:menu.Options(window);  break;
                case 3:menu.Tutorial(window); break;
                //case 2:;  break;
                case 4:window.close(); break;

                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(menuMap);
        window.draw(titulFirst);
        window.draw(titulSecond);
        menu.draw();
        point.setPosition(40, (menu.getSelectedMenuNumber() * 2 + 6) * 32);
        window.draw(point);
        window.display();
    }
    return 0;
}

