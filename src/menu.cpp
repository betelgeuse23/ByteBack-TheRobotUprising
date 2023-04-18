#include "menu.h"

Menu::Menu(sf::RenderWindow& window, sf::String name[], int itm, int xpos, int ypos, int sz, int dstn)
	:menuWindow(window), items(itm), xposition(xpos), yposition(ypos), size(sz), distance(dstn)
{  
	font.loadFromFile("fonts/arial.ttf");
	mainMenu = new sf::Text[items];
    for (int i = 0; i < items; i++, yposition += distance)
        if (i == 0) setInitText(mainMenu[i], name[i], xposition, yposition, 1);
        else setInitText(mainMenu[i], name[i], xposition, yposition, 0);
	mainMenuSelected = 0; 
}

void Menu::setInitText(sf::Text& text, sf::String str, float xpos, float ypos, bool first)
{
	text.setFont(font);
    if (first) text.setFillColor(main_text_color);
    else text.setFillColor(menu_text_color);
	text.setString(str);                
	text.setCharacterSize(size);
	text.setLetterSpacing(2);
	text.setPosition(xpos, ypos);
}

void Menu::draw()
{ 	
	for (int i = 0; i < items; i++) menuWindow.draw(mainMenu[i]);
}

int Menu::getSelectedMenuNumber()
{
	return mainMenuSelected;
}

int Menu::MoveUp()
{
    mainMenu[mainMenuSelected].setFillColor(menu_text_color);
	mainMenuSelected--;
	if (mainMenuSelected == -1) {
		mainMenuSelected = items - 1;
	}
    mainMenu[mainMenuSelected].setFillColor(main_text_color);
	return mainMenuSelected;
}

int Menu::MoveDown()
{
    mainMenu[mainMenuSelected].setFillColor(menu_text_color);
	mainMenuSelected++;
	if (mainMenuSelected == items) {
		mainMenuSelected = 0;
	}
    mainMenu[mainMenuSelected].setFillColor(main_text_color);
	return mainMenuSelected;
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
                if (event.key.code == sf::Keyboard::Escape) return;
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
                if (event.key.code == sf::Keyboard::Escape) return;
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

bool Menu::savingScreen(sf::RenderWindow& window) {

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
            if (event.type == sf::Event::Closed) return 0;
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) return 1;
                if (event.key.code == sf::Keyboard::Return) return 0;
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
    return 0;
}

void Menu::winScreen(sf::RenderWindow& window) {
    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titul("You won!", fontPixel, 80);
    titul.setLetterSpacing(2);
    titul.setFillColor(sf::Color::Magenta);
    titul.setPosition(150, 200);
    sf::Text txt("Press Esc to exit to main menu.", fontArial, 20);
    txt.setLetterSpacing(2);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(250, 300);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) return;
            }
        }
        window.clear();
        window.draw(titul);
        window.draw(txt);
        window.display();
    }
}

void Menu::loseScreen(sf::RenderWindow& window) {
    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titul("You died.", fontPixel, 80);
    titul.setLetterSpacing(2);
    titul.setFillColor(sf::Color::Red);
    titul.setPosition(150, 200);
    sf::Text txt("Press Esc to exit to main menu.", fontArial, 20);
    txt.setLetterSpacing(2);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(250, 300);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) return;
            }
        }
        window.clear();
        window.draw(titul);
        window.draw(txt);
        window.display();
    }
}

void Menu::chooseScreen(sf::RenderWindow& window) {
    sf::Clock clock;
    float delay = 0;

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Text titul("Main game:", fontPixel, 40);
    titul.setFillColor(sf::Color::White);
    titul.setPosition(33, 50);

    sf::String name_lvl[]{ L"lvl 1", L"lvl 2", L"lvl 3", L"lvl 4", L"lvl 5", L"lvl 6", L"lvl 7", L"lvl 8", L"lvl 9", L"lvl 10" };
    Menu menu(window, name_lvl, 10, 96, 128, 25, 32);
    Game game;

    while (window.isOpen()) {
        sf::Event event;

        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        delay += time;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) return;
            }
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
                if (delay > 300) {
                    delay = 0;
                switch (menu.getSelectedMenuNumber())
                {
                case 0:game.setLevel(1);  game.startGame(window, clock, menu);   break;
                case 1:game.setLevel(2);  game.startGame(window, clock, menu);   break;
                case 2:game.setLevel(3);  game.startGame(window, clock, menu);   break;
                case 3:game.setLevel(4);  game.startGame(window, clock, menu);   break;
                case 4:game.setLevel(5);  game.startGame(window, clock, menu);   break;
                case 5:game.setLevel(6);  game.startGame(window, clock, menu);   break;
                case 6:game.setLevel(7);  game.startGame(window, clock, menu);   break;
                case 7:game.setLevel(8);  game.startGame(window, clock, menu);   break;
                case 8:game.setLevel(9);  game.startGame(window, clock, menu);   break;
                case 9:game.setLevel(10);  game.startGame(window, clock, menu);   break;
                }
            }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(menuMap);
        menu.draw();
        window.draw(titul);
        window.display();
    }
}

void Menu::createScreen(sf::RenderWindow& window) {
    sf::Clock clock;
    float delay = 0;

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Text titul("My levels:", fontPixel, 40);
    titul.setFillColor(sf::Color::White);
    titul.setPosition(33, 50);

    std::ifstream file("save.txt");
    std::string line;
    int line_count = 1;
    while (std::getline(file, line)) {
        if (line_count == 4) {
            break;
        }
        line_count++;
    }
    file.close();

    std::stringstream ss(line);
    sf::String name_lvl[3];
    for (int i = 0; i < 3; i++) {
        std::string temp;
        std::getline(ss, temp, ',');
        name_lvl[i] = sf::String(temp);
    }

    Menu menu(window, name_lvl, 3, 96, 128, 25, 32);
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
                /*switch (menu.getSelectedMenuNumber())
                {
                case 0:
                }*/
            }
        }
        window.clear(sf::Color::Black);
        window.draw(menuMap);
        menu.draw();
        window.draw(titul);
        window.display();
    }
}