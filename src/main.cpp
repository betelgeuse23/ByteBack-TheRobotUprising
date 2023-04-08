#include "main.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Metal-Menace", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    sf::Clock clock;
    std::vector<Bullet> bullets;

    // текстуры
    sf::Texture characterTtexture;
    characterTtexture.loadFromFile("images/character.png");
    sf::Sprite player;
    player.setTexture(characterTtexture);
    player.setTextureRect(sf::IntRect(0, 0, 31, 31));
    player.setPosition(0, 0);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/robot1.png");
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(characterTtexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, 31, 31));

    // карта
    const int level[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0,
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

    Entity ent("images/robot2.png", sf::Vector2f(32, 32));


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
            ent.move(Left);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !(isMoving)) {
            isMoving = true;
            dir = 'r';
            ent.move(Right);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !(isMoving)) {
            isMoving = true;
            dir = 'u';
            ent.move(Up);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !(isMoving)) {
            isMoving = true;
            dir = 'd';
            ent.move(Down);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (bulDelay > 300)
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
        for (auto& elem : bullets) { 
            bulletSprite.setPosition(elem.update(0.09 * time));
            window.draw(bulletSprite);
        }
        /*ent.move();
        ent.draw(window);*/
        window.draw(player);
        window.display();
    }

    return 0;
}