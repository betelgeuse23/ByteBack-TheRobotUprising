#include "main.h"

void Options();
void Tutorial();
void savingScreen();

void startGame(sf::Clock &clock) {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    int lives = 5;

    // текстуры
    sf::Texture characterTexture;
    characterTexture.loadFromFile("images/character.png");
    sf::Sprite player;
    player.setTexture(characterTexture);
    player.setTextureRect(sf::IntRect(0, 0, 31, 31));
    player.setPosition(32, 32);

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
    map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH);

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
                if (event.key.code == sf::Keyboard::Escape) {
                    savingScreen();
                    window.close();
                }
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

        if (isMoving && dir == 'n' && delay / 1000 > 0.1) isMoving = false;

        if (isMoving&& move < 32 && dir != 'n') {
            move += 0.2 * time;
            float k = move > 32 ? move - 32 : 0;
            if (dir == 'u') {
                player.move(0, -0.2 * time + k);
            }
            else if (dir == 'd') {
                player.move(0, 0.2 * time - k);
            }
            else if (dir == 'r') {
                player.move(0.2 * time - k, 0);
            }
            else if (dir == 'l') {
                player.move(-0.2 * time + k, 0);
            }
            player.setTextureRect(sf::IntRect(32 * (int(move / 16) % 2), 0, 31, 31));
        }
        else {
            move = 0;
            dir = 'n';
            player.setTextureRect(sf::IntRect(0, 0, 31, 31));
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(healthText);
        for (int i = 0; i < lives; i++)
        {
            heartSprite.setPosition(620 + (i + 1) * 32 , 20);
            window.draw(heartSprite);
        }

        for (auto& bullet : bullets) {
            bullet.update(0.09 * time);
            if (bullet.checkCollisionsWithEnemy(enemies)) {
                bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
            }
            if (bullet.checkCollisionsWithObstacle(level)) {
                bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH);
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
}

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
                case 0:startGame(clock);   break;
                case 2:Options();  break;
                case 3:Tutorial(); break;
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

void Options() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    sf::Clock clock;
    float delay = 0;

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }
        window.clear();
        window.draw(menuMap);
        window.display();
    }
}

void Tutorial() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text plotTitul("Plot", fontPixel, 60);
    plotTitul.setLetterSpacing(2);
    plotTitul.setFillColor(sf::Color::White);
    plotTitul.setPosition(32, 32);

    sf::Text plotText1("You are a retired intelligence sergeant who has been reinstated and sent on a mission", fontArial, 20);
    plotText1.setFillColor(sf::Color::White);
    plotText1.setPosition(32, 120);

    sf::Text plotText2("to save the city from a robot invasion.These robots were once loyal to humans and", fontArial, 20);
    plotText2.setFillColor(sf::Color::White);
    plotText2.setPosition(32, 150);

    sf::Text plotText3("helped them in their daily lives, but have been reprogrammed by a group of hackers", fontArial, 20);
    plotText3.setFillColor(sf::Color::White);
    plotText3.setPosition(32, 180);

    sf::Text plotText4("who want to take over the city.Your task is to protect the computing cluster until", fontArial, 20);
    plotText4.setFillColor(sf::Color::White);
    plotText4.setPosition(32, 210);

    sf::Text plotText5("your colleagues can take back control of the robots.", fontArial, 20);
    plotText5.setFillColor(sf::Color::White);
    plotText5.setPosition(32, 240);

    sf::Text EnemyTitul("Beware!", fontPixel, 40);
    EnemyTitul.setLetterSpacing(1);
    EnemyTitul.setFillColor(sf::Color::White);
    EnemyTitul.setPosition(110, 310);

    sf::Text BonusTitul("Grab it!", fontPixel, 40);
    BonusTitul.setLetterSpacing(1);
    BonusTitul.setFillColor(sf::Color::White);
    BonusTitul.setPosition(510, 310);

    sf::Texture bonusTexture;
    bonusTexture.loadFromFile("images/bonusesbig.png");
    sf::Sprite bonuses;
    bonuses.setTexture(bonusTexture);
    bonuses.setPosition(470, 380);

    sf::Texture robot1Texture;
    robot1Texture.loadFromFile("images/robot1big.png");
    sf::Sprite robot1;
    robot1.setTexture(robot1Texture);
    robot1.setPosition(20, 380);

    sf::Texture robot2Texture;
    robot2Texture.loadFromFile("images/robot2big.png");
    sf::Sprite robot2;
    robot2.setTexture(robot2Texture);
    robot2.setPosition(160, 380);

    sf::Texture robot3Texture;
    robot3Texture.loadFromFile("images/robot3big.png");
    sf::Sprite robot3;
    robot3.setTexture(robot3Texture);
    robot3.setPosition(300, 380);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }
        window.clear();
        window.draw(menuMap);
        window.draw(plotTitul);
        window.draw(plotText1);
        window.draw(plotText2);
        window.draw(plotText3);
        window.draw(plotText4);
        window.draw(plotText5);
        window.draw(EnemyTitul);
        window.draw(BonusTitul);
        window.draw(bonuses);
        window.draw(robot1);
        window.draw(robot2);
        window.draw(robot3);
        window.display();
    }
}

void savingScreen() {
    sf::RenderWindow window(sf::VideoMode((WIDTH + 6) * 32, WIDTH * 32), "Byte Back: The Robot Uprising", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titulEnter("Enter", fontPixel, 50);
    titulEnter.setLetterSpacing(2);
    titulEnter.setFillColor(sf::Color::White);
    titulEnter.setPosition(20, 10);
    sf::Text txtEnter("Press Enter to save and exit.", fontArial, 30);
    txtEnter.setLetterSpacing(2);
    txtEnter.setFillColor(sf::Color::White);
    txtEnter.setPosition(20, 60);

    sf::Text titulReturn("Return", fontPixel, 50);
    titulReturn.setLetterSpacing(2);
    titulReturn.setFillColor(sf::Color::White);
    titulReturn.setPosition(20, 100);
    sf::Text txtReturn("Press Esc to continue game.", fontArial, 30);
    txtReturn.setLetterSpacing(2);
    txtReturn.setFillColor(sf::Color::White);
    txtReturn.setPosition(20, 150);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Return) window.close();
            }
        }
        window.clear();
        window.draw(menuMap);
        window.draw(titulEnter);
        window.draw(txtEnter);
        window.draw(titulReturn);
        window.draw(txtReturn);
        window.display();
    }
}