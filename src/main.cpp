#include "main.h"

int main() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 8) * 32, WIDTH * 32), "Metal-Menace", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    sf::Clock clock;
    std::vector<Bullet> bullets;

    // текстуры
    sf::Texture characterTexture;
    characterTexture.loadFromFile("images/character.png");
    sf::Sprite player;
    player.setTexture(characterTexture);
    player.setTextureRect(sf::IntRect(0, 0, 31, 31));
    player.setPosition(0, 0);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/bullet.png");
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

    // текст
    sf::Font font;
    font.loadFromFile("fonts/pixel.ttf");

    sf::Text healthText("Health", font, 32);
    healthText.setLetterSpacing(2);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(680, 10);

    sf::Text enemiesText("Enemies", font, 32);
    enemiesText.setLetterSpacing(2);
    enemiesText.setFillColor(sf::Color::White);
    enemiesText.setPosition(680, 200);

    // карта
    int level[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    TileMap map;
    if (!map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH))
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
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();

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
            if (bulDelay > 800)
            {
                isShooting = true;
                Bullet bullet(player.getPosition(), dir);
                bullets.push_back(bullet);
            }
            else isShooting = false;
        }

        if (isMoving && dir == 'n' && delay/1000 > 0.1) isMoving = false;

        if (isMoving && move < 32 && dir != 'n') {
            move += 0.2 * time;
            float k = move > 32 ? move - 32 : 0;
            if (dir == 'u') {
                player.move(0, -0.2 * time + k);
            } else if (dir == 'd') {
                player.move(0, 0.2 * time - k);
            } else if (dir == 'r') {
                player.move(0.2 * time - k, 0);
            } else if (dir == 'l') {
                player.move(-0.2 * time + k, 0);
            }
            player.setTextureRect(sf::IntRect(32 * (int(move/16) % 2), 0, 31, 31));
        }
        else {
            move = 0;
            dir = 'n';
            player.setTextureRect(sf::IntRect(0, 0, 31, 31));
        }
        
        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(healthText);
        window.draw(enemiesText);

        for (auto& bullet : bullets) {
            bullet.update(0.09 * time);
            //if (bullet.checkCollisionsWithEnemy(enemies)) {
            //    // bullet hit an enemy, remove both bullet and enemy
            //    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
            //}
            if (bullet.checkCollisionsWithObstacle(level)) {
                bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                if (!map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH))
                    return -1;
            }
            else if (bullet.getPosition().y < 0 && bullet.getDirection() == 'u' || 
                bullet.getPosition().y > 672 && (bullet.getDirection() == 'd' || bullet.getDirection() == 'n') ||
                bullet.getPosition().x < 0 && bullet.getDirection() == 'l' ||
                bullet.getPosition().x > 672 && bullet.getDirection() == 'r') {
                std::cout << 'a';
                bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
            }
            else {
                bulletSprite.setPosition(bullet.getPosition());
                window.draw(bulletSprite);
            }
        }
        window.draw(player);
        window.display();
    }

    return 0;
}