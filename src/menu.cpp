#include "menu.h"

Menu::Menu(sf::RenderWindow& window, sf::String name[])
	:menuWindow(window)
{  
	font.loadFromFile("fonts/arial.ttf");
	mainMenu = new sf::Text[5];
	for (int i = 0, ypos = 192; i < 5; i++, ypos += 64) setInitText(mainMenu[i], name[i], 96, ypos);
	mainMenuSelected = 0; 
}

void Menu::setInitText(sf::Text& text, sf::String str, float xpos, float ypos)
{
	text.setFont(font);
	text.setFillColor(menu_text_color);
	text.setString(str);                
	text.setCharacterSize(30);
	text.setLetterSpacing(2);
	text.setPosition(xpos, ypos);
}

void Menu::draw()
{ 	
	for (int i = 0; i < 5; i++) menuWindow.draw(mainMenu[i]);
}

int Menu::getSelectedMenuNumber()
{
	return mainMenuSelected;
}

int Menu::MoveUp()
{
	mainMenuSelected--;
	if (mainMenuSelected == -1) {
		mainMenuSelected = 4;
	}
	return mainMenuSelected;
}

int Menu::MoveDown()
{
	mainMenuSelected++;
	if (mainMenuSelected == 5) {
		mainMenuSelected = 0;
	}
	return mainMenuSelected;
}

void Menu::startGame(sf::RenderWindow& window, sf::Clock& clock) {

    // ��������
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

    // �����
    sf::Font font;
    font.loadFromFile("fonts/pixel.ttf");

    sf::Text healthText("Health", font, 32);
    healthText.setLetterSpacing(1);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(680, 10);

    // �����
    int level[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        3, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 3,
        3, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 3,
        3, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 3,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 5, 2, 2, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 5, 2, 6, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TileMap map;
    map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH);

    bool isShooting = false;
    float bulDelay = 0;

    Level level1;
    level1.size = sf::Vector2i(21, 21);
    level1.map = level;
    level1.base = sf::Vector2i(10, 20);
    level1.player = new Player("images/character.png", sf::Vector2i(0, 0));
    level1.player->initPatfind(&level1, { {0,1}, {3,1}, {6,1} });
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 1)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 2)));
    level1.enemies.push_back(new Enemy("images/robot2.png", sf::Vector2i(1, 3)));
    for (auto& e : level1.enemies) {
        e->initPatfind(&level1, { {0,1}, {6, 1}, {1, 2}, {2, 3}, {4, 1} });
        e->makeTarget();
    }

    float del = 0;


    // ������ � �����
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();

        del += time;

        if (isShooting) bulDelay = 0;
        else bulDelay += time;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // ��������� ����
                if (event.key.code == sf::Keyboard::Escape) {
                    savingScreen(window);
                    window.close();
                }
            }
        }

        // ��������� ������� ������
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            level1.player->move(Left);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            level1.player->move(Right);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            level1.player->move(Up);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            level1.player->move(Down);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (bulDelay > 500)
            {
                isShooting = true;
                level1.bullets.push_back(new Bullet(level1.player->getPosition() + Utils::makeDir(level1.player->getLastDirection()), level1.player->getLastDirection(), level1.player->getDamage()));
            }
            else isShooting = false;
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.draw(healthText);

        for (auto& e : level1.enemies) {
            if (e->getState() != Dead) {
                e->update();
                e->draw(window);
                if (e->getPosition() == level1.base) {
                    level1.player->doDamage();
                    level1.player->doDamage();
                    level1.player->doDamage();
                    level1.player->doDamage();
                    level1.player->doDamage();
                }
            }
            else {
                delete e;
                level1.enemies.erase(std::find(level1.enemies.begin(), level1.enemies.end(), e));
            }
        }

        level1.player->update();
        level1.player->draw(window);

        for (auto& bullet : level1.bullets) {
            bullet->update(time);
            if (bullet->checkCollisions(&level1)) {
                level1.bullets.erase(std::find(level1.bullets.begin(), level1.bullets.end(), bullet));
                map.load("images/tileset.png", sf::Vector2u(32, 32), level, WIDTH, WIDTH);
            }
            else {
                bulletSprite.setPosition(bullet->getPosition());
                window.draw(bulletSprite);
            }
        }

        for (int i = 0; i < level1.player->getLives(); i++)
        {
            heartSprite.setPosition(620 + (i + 1) * 32, 20);
            window.draw(heartSprite);
        }
        if (level1.player->getLives() == 0 || level1.enemies.empty()) {
            window.close();
        }

        window.display();
    }
}

void Menu::Options(sf::RenderWindow& window) {
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

void Menu::Tutorial(sf::RenderWindow& window) {

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
    BonusTitul.setPosition(530, 310);

    sf::Text RememberTitul("Remember!", fontPixel, 40);
    RememberTitul.setLetterSpacing(1);
    RememberTitul.setFillColor(sf::Color::White);
    RememberTitul.setPosition(32, 510);

    sf::Text RememberText1("You can walk on bars, but not on water.", fontArial, 20);
    RememberText1.setFillColor(sf::Color::White);
    RememberText1.setPosition(32, 570);

    sf::Text RememberText2("Your enemies can walk on water, but they can't walk on bars.", fontArial, 20);
    RememberText2.setFillColor(sf::Color::White);
    RememberText2.setPosition(32, 600);

    sf::Texture bonusTexture;
    bonusTexture.loadFromFile("images/bonusesbig.png");
    sf::Sprite bonuses;
    bonuses.setTexture(bonusTexture);
    bonuses.setPosition(500, 380);

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
        window.draw(RememberTitul);
        window.draw(RememberText1);
        window.draw(RememberText2);
        window.draw(bonuses);
        window.draw(robot1);
        window.draw(robot2);
        window.draw(robot3);
        window.display();
    }
}

void Menu::savingScreen(sf::RenderWindow& window) {

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
    sf::Text txtEnter("Press Esc to save and exit.", fontArial, 30);
    txtEnter.setLetterSpacing(2);
    txtEnter.setFillColor(sf::Color::White);
    txtEnter.setPosition(20, 60);

    sf::Text titulReturn("Return", fontPixel, 50);
    titulReturn.setLetterSpacing(2);
    titulReturn.setFillColor(sf::Color::White);
    titulReturn.setPosition(20, 100);
    sf::Text txtReturn("Press Enter to continue game.", fontArial, 30);
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