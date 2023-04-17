#include "game.h"

void Game::startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu) {

    // текстуры
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/bullet.png");
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

    sf::Texture heartTexture;
    heartTexture.loadFromFile("images/bonusesbig.png");
    sf::Sprite heartSprite;
    heartSprite.setTexture(heartTexture);
    heartSprite.setTextureRect(sf::IntRect(0, 0, 95, 95));

    // текст
    sf::Font font;
    font.loadFromFile("fonts/pixel.ttf");

    sf::Text healthText("Health", font, 32);
    healthText.setLetterSpacing(1);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(680, 10);

    int* level = Map::getMap(10);
    TileMap map;
    map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH);

    Level level1;
    level1.size = sf::Vector2i(21, 21);
    level1.translate(level);
    level1.base = sf::Vector2i(10, 19);

    PathFinder pfE(&level1, { {0,1}, {6, 1}, {1, 2}, {2, 3}, {4, 1} });
    PathFinder pfP(&level1, { {0,1}, {3,1}, {6,1} });

    level1.players.push_back(new Player("images/character.png", sf::Vector2i(10, 10)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 1)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 2)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 3)));

    for (auto& e : level1.enemies) {
        e->initPatfind(&pfE);
        e->makeTarget();
    }

    for (auto& p : level1.players) {
        p->initPatfind(&pfP);
    }


    // работа с окном
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // Закрываем окно
                if (event.key.code == sf::Keyboard::Escape) {
                    if (menu.savingScreen(window)) return;
                }
            }
        }

        // обработка нажатий клавиш
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            level1.players[0]->move(Left);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            level1.players[0]->move(Right);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            level1.players[0]->move(Up);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            level1.players[0]->move(Down);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            level1.players[0]->shoot(&level1);
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(healthText);

        for (auto& e : level1.enemies) {
            if (e->getState() != Dead) {
                e->update();
                e->draw(window);
                if (e->getPosition() == level1.base) { level1.players[0]->doDamage(5); e->setState(State::Dead); }
            }
            else {
                delete e;
                level1.enemies.erase(std::find(level1.enemies.begin(), level1.enemies.end(), e));
            }
        }

        for (auto& p : level1.players) {
            p->update();
            p->draw(window);
        }

        for (auto& bullet : level1.bullets) {
            if (bullet->checkCollisions(&level1)) {
                level1.bullets.erase(std::find(level1.bullets.begin(), level1.bullets.end(), bullet));
                map.load("images/tileset.png", sf::Vector2u(cell, cell), level1.map, WIDTH, WIDTH);
            }
            else {
                bullet->update(time);
                bulletSprite.setPosition(bullet->getPosition());
                window.draw(bulletSprite);
            }
        }

        for (int i = 0; i < level1.players[0]->getLives(); i++)
        {
            heartSprite.setPosition(620 + (i + 1) * 32, 20);
            window.draw(heartSprite);
        }
        if (level1.players[0]->getLives() == 0 || level1.enemies.empty()) return;

        window.display();
    }
}