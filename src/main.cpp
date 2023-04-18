#include "main.h"

int main() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    sf::Clock clock;
    float delay = 0;
    int gameLevel = 0;

    // текст
    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titulFirst("Byte Back:", fontPixel, 80);
    titulFirst.setLetterSpacing(2);
    titulFirst.setFillColor(sf::Color::White);
    titulFirst.setPosition(27, 44);
    sf::Text titulSecond("the robot uprising", fontArial, 30);
    titulSecond.setLetterSpacing(2);
    titulSecond.setFillColor(sf::Color::White);
    titulSecond.setPosition(420, 128);
    sf::Text point(">", fontPixel, 35);
    point.setFillColor(sf::Color::White);

    sf::Texture menuTexture;
    menuTexture.loadFromFile("images/menu.png");
    sf::Sprite menuSprite;
    menuSprite.setTexture(menuTexture);

    sf::String name_menu[]{ L"NEW GAME", L"CONTINUE", L"CHOOSE LEVEL", L"MULTIPLAYER", L"CREATE LEVEL", L"OPTIONS", L"TUTORIAL", L"QUIT"};
    Menu menu(window, name_menu, 8, 96, 160, 30, 64);
    Game game;

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
                case 0:game.setLevel(1);  game.startGame(window, clock, menu);   break;
                case 1: {
                    std::ifstream file("save.txt");
                    std::string lvl;
                    getline(file, lvl);
                    game.setLevel(std::stoi(lvl));
                    game.startGame(window, clock, menu);
                    break;
                }
                case 2:menu.chooseScreen(window); break;
                    //
                case 4:menu.createScreen(window); break;
                case 5:menu.Options(window);  break;
                case 6:menu.Tutorial(window); break;
                case 7:window.close(); break;

                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(menuSprite);
        window.draw(titulFirst);
        window.draw(titulSecond);
        menu.draw();
        point.setPosition(40, (menu.getSelectedMenuNumber() * 2 + 5) * 32);
        window.draw(point);
        window.display();
    }
    return 0;
}

