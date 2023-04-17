#include "game.h"

void Game::startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu) {
    std::srand(time(0));

    // текстуры
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/bullet.png");
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

    sf::Texture shieldTexture;
    shieldTexture.loadFromFile("images/shield.png");
    sf::Sprite shieldSprite;
    shieldSprite.setTexture(shieldTexture);
    shieldSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

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

    int* lvl = Map::getMap(level);
    TileMap map;
    map.load("images/tileset.png", sf::Vector2u(32, 32), lvl, WIDTH, WIDTH);

    Level level1;
    level1.size = sf::Vector2i(21, 21);
    level1.translate(lvl);
    level1.base = sf::Vector2i(10, 20);

    PathFinder pfE(&level1, { {0,1}, {6, 1}, {1, 2}, {2, 3}, {3, 1} });
    PathFinder pfP(&level1, { {0,1}, {4,1}, {6,1} });

    level1.players.push_back(new Player("images/character.png", sf::Vector2i(10, 10)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(2, 2)));
    /*level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(2, 3)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(3, 2)));*/
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(3, 3)));
    level1.bonuses.push_back(new Bonus(Health, sf::Vector2i(7, 5)));
    level1.bonuses.push_back(new Bonus(Speed, sf::Vector2i(8, 5)));
    level1.bonuses.push_back(new Bonus(Rate, sf::Vector2i(9, 5)));
    level1.bonuses.push_back(new Bonus(Shield, sf::Vector2i(10, 5)));
    level1.bonuses.push_back(new Bonus(Damage, sf::Vector2i(11, 5)));
    level1.bonuses.push_back(new Bonus(Spare, sf::Vector2i(12, 5)));

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

        level1.bullets.erase(std::remove_if(level1.bullets.begin(), level1.bullets.end(), [&level1](Bullet* ptr) { return ptr->checkCollisions(&level1); }), level1.bullets.end());
        level1.enemies.erase(std::remove_if(level1.enemies.begin(), level1.enemies.end(), [](Enemy* ptr) { return ptr->getState() == Dead; }), level1.enemies.end());
        level1.bonuses.erase(std::remove_if(level1.bonuses.begin(), level1.bonuses.end(), [&level1](Bonus* ptr) { return ptr->checkCollisions(&level1); }), level1.bonuses.end());

        map.load("images/tileset.png", sf::Vector2u(cell, cell), level1.map, WIDTH, WIDTH);

        for (auto& b : level1.bonuses) {
            b->draw(window);
        }

        for (auto& e : level1.enemies) {
            if (e->getState() != Dead) {
                e->update();
                e->draw(window);
                if (e->getPosition() == level1.base) {
                    level1.players[0]->doDamage(-5);
                    e->setState(State::Dead);
                }
            }
        }

        for (auto& p : level1.players) {
            p->update();
            p->draw(window);
            if (p->isShielded()) {
                shieldSprite.setPosition(p->getSpritePosition());
                shieldSprite.setTextureRect(sf::IntRect(32*(std::rand()%4), 0, 31, 31));
                window.draw(shieldSprite);
            }
        }

        for (auto& b : level1.bullets) {
            b->update(time);
            bulletSprite.setPosition(b->getPosition());
            window.draw(bulletSprite);
        }

        for (int i = 0; i < level1.players[0]->getLives(); i++) {
            heartSprite.setPosition(620 + (i + 1) * 32, 20);
            window.draw(heartSprite);
        }
        if (level1.players[0]->getLives() == 0) {
            menu.loseScreen(window);
            return;
        }
        if (level1.enemies.empty()) {
            if (level == 10) {
                level = 1;
                menu.winScreen(window);
                return;
            }
            else {
                level++;
                std::fstream file("save.txt", std::ios::in | std::ios::out);
                file.seekp(0);
                file << std::to_string(level) << std::endl;
                Game game;
                game.setLevel(level);
                game.startGame(window, clock, menu);
            }
        }

        window.display();
    }
}