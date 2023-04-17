#include "game.h"

void Game::startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu) {
    std::srand(time(0));

    // ��������
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

    // �����
    sf::Font font;
    font.loadFromFile("fonts/pixel.ttf");

    sf::Text healthText("Health", font, 32);
    healthText.setLetterSpacing(1);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(680, 10);

    int* lvl = Map::getMap(level);
    TileMap map;

    Level level1;
    level1.size = sf::Vector2i(WIDTH, WIDTH);
    level1.translate(lvl);
    Spawner sp(&level1);
    sp.spawn(lvl);

    // ������ � �����
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
                // ��������� ����
                if (event.key.code == sf::Keyboard::Escape) {
                    if (menu.savingScreen(window)) return;
                }
            }
        }

        // ��������� ������� ������
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
            if (p->getEffect() == Shield) {
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