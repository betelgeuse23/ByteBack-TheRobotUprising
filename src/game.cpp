#include "game.h"

void Game::startGame(sf::RenderWindow& window, sf::Clock& clock, Menu& menu) {
    std::srand(time(0));

    sf::Music music;
    music.openFromFile("sounds/gameSong.wav");
    music.setLoop(true);
    music.play();

    // ��������
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/bullet.png");
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

    sf::Texture effectTexture;
    effectTexture.loadFromFile("images/effects.png");
    sf::Sprite effectSprite;
    effectSprite.setTexture(effectTexture);

    sf::Texture heartTexture;
    heartTexture.loadFromFile("images/hearts.png");
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
        if (!level1.players.empty()) {
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
            if (level1.players.size() == 2) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    level1.players[1]->move(Left);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    level1.players[1]->move(Right);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    level1.players[1]->move(Up);
                } 
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    level1.players[1]->move(Down);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    level1.players[1]->shoot(&level1);
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(healthText);

        level1.bullets.erase(std::remove_if(level1.bullets.begin(), level1.bullets.end(), [&level1](Bullet* ptr) { return ptr->checkCollisions(&level1); }), level1.bullets.end());
        level1.enemies.erase(std::remove_if(level1.enemies.begin(), level1.enemies.end(), [](Enemy* ptr) { return ptr->getState() == Dead; }), level1.enemies.end());
        level1.players.erase(std::remove_if(level1.players.begin(), level1.players.end(), [](Player* ptr) { return ptr->getState() == Dead; }), level1.players.end());
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

        int hI = 0;
        for (auto& p : level1.players) {
            p->update();
            if(p->getState() != Dead) p->draw(window);
            if (p->getEffect() != Spare) {
                effectSprite.setTextureRect(sf::IntRect(32 * (std::rand() % 4), 32 * (p->getEffect() - 1), 31, 31));
                effectSprite.setPosition(p->getSpritePosition());
                window.draw(effectSprite);
            }
            for (int i = 0; i < p->getLives(); i++) {
                heartSprite.setTextureRect(sf::IntRect(p->getColor() * 95, 0, 95, 95));
                heartSprite.setPosition(620 + (i + 1) * 32, 20 + 95 * hI);
                window.draw(heartSprite);
            }
            hI++;
        }

        for (auto& b : level1.bullets) {
            b->update(time);
            bulletSprite.setPosition(b->getPosition());
            window.draw(bulletSprite);
        }

        if (level1.players.empty()) {
            level1.pl.playLoose();
            menu.loseScreen(window);
            return;
        }

        if (level1.enemies.empty()) {
            music.stop();
            level1.pl.playWin();
            if (level == 10) {
                level = 1;
                menu.winScreen(window);
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
            return;
        }

        window.display();
    }
}