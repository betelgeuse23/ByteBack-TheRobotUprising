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

void Menu::ColorChoose(sf::RenderWindow& window, struct Options& opts) {
    sf::Clock clock;
    float delay = 0;

    sf::Texture p1T;
    p1T.loadFromFile("images/player1big.png");
    sf::Sprite p1S;
    p1S.setTexture(p1T);

    sf::Texture p2T;
    p2T.loadFromFile("images/player2big.png");
    sf::Sprite p2S;
    p2S.setTexture(p2T);

    sf::Texture p3T;
    p3T.loadFromFile("images/player3big.png");
    sf::Sprite p3S;
    p3S.setTexture(p3T);

    sf::Texture p4T;
    p4T.loadFromFile("images/player4big.png");
    sf::Sprite p4S;
    p4S.setTexture(p4T);

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Text titul("      Choose your colour: \n\n   W/S               UP/DOWN", fontPixel, 40);
    titul.setFillColor(sf::Color::White);
    titul.setPosition(33, 50);

    sf::String name_lvl[]{ L"0", L"1", L"2", L"3" };
    Menu menu1(window, name_lvl, 4, 96, 128, 25, 32);
    Menu menu2(window, name_lvl, 4, 96, 128, 25, 32);
    menu2.MoveDown();
    Game game;

    opts.cols = sf::Vector2i(0, 1);

    while (window.isOpen()) {
        sf::Event event;

        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        delay += time;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) {
                    return;
                }
            }
            if (event.key.code == sf::Keyboard::Up) {
                if (delay > 300)
                {
                    menu2.MoveUp();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::Down) {
                if (delay > 300)
                {
                    menu2.MoveDown();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::W) {
                if (delay > 300)
                {
                    menu1.MoveUp();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::S) {
                if (delay > 300)
                {
                    menu1.MoveDown();
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::Return)
            {
                if (delay > 300) {
                    delay = 0;
                    opts.cols = sf::Vector2i(menu1.getSelectedMenuNumber(), menu2.getSelectedMenuNumber());
                    return;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(menuMap);

        switch (menu1.getSelectedMenuNumber()) {
        case 0:
            p1S.setPosition(120, 250);
            window.draw(p1S);
            break;
        case 1:
            p2S.setPosition(120, 250);
            window.draw(p2S);
            break;
        case 2:
            p3S.setPosition(120, 250);
            window.draw(p3S);
            break;
        case 3:
            p4S.setPosition(120, 250);
            window.draw(p4S);
            break;
        }
        switch (menu2.getSelectedMenuNumber()) {
        case 0:
            p1S.setPosition(600, 250);
            window.draw(p1S);
            break;
        case 1:
            p2S.setPosition(600, 250);
            window.draw(p2S);
            break;
        case 2:
            p3S.setPosition(600, 250);
            window.draw(p3S);
            break;
        case 3:
            p4S.setPosition(600, 250);
            window.draw(p4S);
            break;
        }

        window.draw(titul);
        window.display();
    }
}

void Menu::Options(sf::RenderWindow& window, sf::Music& mus, struct Options& opts) {
    sf::Clock clock;
    float delay = 0;

    TileMap menuMap;
    int map[729] = { 0 };
    menuMap.load("images/tileset.png", sf::Vector2u(32, 32), map, WIDTH + 6, WIDTH);

    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Text titul("Options:", fontPixel, 40);
    titul.setFillColor(sf::Color::White);
    titul.setPosition(33, 50);

    sf::Text ok("#", fontPixel, 40);

    sf::String name_lvl[]{ L"Toggle Music", L"Toggle Sound" };
    Menu menu(window, name_lvl, 2, 96, 128, 25, 32);
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
                if (event.key.code == sf::Keyboard::Escape) {
                    return;
                }
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
                    switch (menu.getSelectedMenuNumber()) {
                    case 0:
                        opts.music = opts.music ? 0 : 1;
                        if (opts.music) mus.play();
                        else mus.stop();
                        opts.setOpts();
                        break;
                    case 1:
                        opts.sound = opts.sound ? 0 : 1;
                        opts.setOpts(); 
                        break;
                    }
                }
            }
        }
        
        window.clear(sf::Color::Black);
        window.draw(menuMap);

        if (opts.music) {
            if (menu.getSelectedMenuNumber() == 0) ok.setFillColor(sf::Color::Yellow);
            else ok.setFillColor(sf::Color::White);
            ok.setPosition(322, 122);
            window.draw(ok);
        }
        if (opts.sound) {
            if (menu.getSelectedMenuNumber() == 1) ok.setFillColor(sf::Color::Yellow);
            else ok.setFillColor(sf::Color::White);
            ok.setPosition(322, 154);
            window.draw(ok);
        }
        
        menu.draw();
        window.draw(titul);
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

void Menu::chooseScreen(sf::RenderWindow& window, struct Options& opts) {
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
    sf::Text mult("Toggle multipeer (M)", fontPixel, 25);
    mult.setFillColor(sf::Color::White);
    mult.setPosition(33, 607);
    sf::Text ok("#", fontPixel, 40);
    ok.setFillColor(sf::Color::White);
    ok.setPosition(354,603);

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
                if (event.key.code == sf::Keyboard::Escape) {
                    opts.players = 1;
                    return;
                }
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
            if (event.key.code == sf::Keyboard::M) {
                if (delay > 300)
                {
                    opts.players = opts.players==2?1:2;
                    delay = 0;
                }
            }
            if (event.key.code == sf::Keyboard::Return)
            {
                if (delay > 300) {
                    delay = 0;
                    switch (menu.getSelectedMenuNumber()) {
                        case 0:game.setLevel(1);  game.startGame(window, clock, menu, opts);   break;
                        case 1:game.setLevel(2);  game.startGame(window, clock, menu, opts);   break;
                        case 2:game.setLevel(3);  game.startGame(window, clock, menu, opts);   break;
                        case 3:game.setLevel(4);  game.startGame(window, clock, menu, opts);   break;
                        case 4:game.setLevel(5);  game.startGame(window, clock, menu, opts);   break;
                        case 5:game.setLevel(6);  game.startGame(window, clock, menu, opts);   break;
                        case 6:game.setLevel(7);  game.startGame(window, clock, menu, opts);   break;
                        case 7:game.setLevel(8);  game.startGame(window, clock, menu, opts);   break;
                        case 8:game.setLevel(9);  game.startGame(window, clock, menu, opts);   break;
                        case 9:game.setLevel(10);  game.startGame(window, clock, menu, opts);   break;
                    }
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(menuMap);
        menu.draw();
        window.draw(mult);
        if(opts.players == 2) window.draw(ok);
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
    sf::String name_lvl[4];
    for (int i = 0; i < 3; i++) {
        std::string temp;
        std::getline(ss, temp, ',');
        name_lvl[i] = sf::String(temp);
    }
    name_lvl[3] = "CREATE A LEVEL";

    Menu menu(window, name_lvl, 4, 96, 128, 25, 32);
    Game game;

    while (window.isOpen()) {
        sf::Event event;

        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        delay += time;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
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
                switch (menu.getSelectedMenuNumber())
                {
                case 3: menu.levelScreen(window, clock, menu); break;
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

void Menu::levelScreen(sf::RenderWindow& window, sf::Clock& clock, Menu menu) {
    std::srand(time(0));
    float delay = 0;

    sf::Font font;
    font.loadFromFile("fonts/pixel.ttf");

    sf::Text blocksText("Blocks", font, 32);
    blocksText.setLetterSpacing(1);
    blocksText.setFillColor(sf::Color::White);
    blocksText.setPosition(680, 10);

    sf::Texture blocksTexture;
    blocksTexture.loadFromFile("images/tilesetLvl.png");
    sf::Sprite blocks;
    blocks.setTexture(blocksTexture);
    blocks.setPosition(673, 64);

    sf::Texture frameTexture;
    frameTexture.loadFromFile("images/frame.png");
    sf::Sprite frame;
    frame.setTexture(frameTexture);
    frame.setPosition(673, 64);

    TileMap menuMap;
    int map[441] = { 0 };
    map[0] = 45;
    
    Level level1;
    level1.size = sf::Vector2i(WIDTH, WIDTH);
    Spawner sp(&level1);
    sp.spawn(map);

    int selectedBlock = 0;

    // работа с окном
    while (window.isOpen())
    {
        sf::Event event;
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        delay += time;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // Закрываем окно
                if (event.key.code == sf::Keyboard::Escape) return;
            }
        }

        // обработка нажатий клавиш
        if (!level1.enemies.empty()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                level1.enemies[0]->move(Left);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                level1.enemies[0]->move(Right);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                level1.enemies[0]->move(Up);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                level1.enemies[0]->move(Down);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                int temp = (Utils::vPos(level1.enemies[0]->getPosition()));
                if (selectedBlock == 0) map[temp] = selectedBlock;
                else map[temp] = selectedBlock + 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                if (delay > 300)
                {
                    (selectedBlock == 0) ? selectedBlock = 5 : selectedBlock--;
                    delay = 0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                if (delay > 300)
                {
                    (selectedBlock == 5) ? selectedBlock = 0 : selectedBlock++;
                    delay = 0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                if (delay > 300)
                {
                    fileNameScreen(window, map);
                    delay = 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        level1.translate(map);
        menuMap.load("images/tileset.png", sf::Vector2u(32, 32), level1.map, WIDTH, WIDTH);
        window.draw(menuMap);
        window.draw(blocksText);
        window.draw(blocks);
        frame.setPosition(673 + selectedBlock * 32, 64);
        window.draw(frame);

        for (auto& p : level1.enemies) {
            p->update();
            p->draw(window);
        }

        window.display();
    }
}

void Menu::fileNameScreen(sf::RenderWindow& window, int* map) {
   
    sf::Font fontPixel;
    fontPixel.loadFromFile("fonts/pixel.ttf");
    sf::Font fontArial;
    fontArial.loadFromFile("fonts/arial.ttf");

    sf::Text titulEnter("Write file name", fontPixel, 50);
    titulEnter.setLetterSpacing(2);
    titulEnter.setFillColor(sf::Color::White);
    titulEnter.setPosition(20, 10);
    sf::Text txtEnter("Press Enter to save and exit.", fontArial, 30);
    txtEnter.setLetterSpacing(2);
    txtEnter.setFillColor(sf::Color::White);
    txtEnter.setPosition(20, 60);

    sf::RectangleShape inputBox(sf::Vector2f(200, 30));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setPosition(300, 300);

    sf::Text inputText;
    inputText.setFont(fontArial);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(310, 305);

    std::string userInput;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed);
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Return) {
                    std::stringstream aa(userInput);
                    std::vector<std::string> words;
                    std::string word;

                    while (aa >> word) {
                        words.push_back(word);
                    }

                    for (const auto& w : words) {
                        std::cout << w << std::endl;
                    }

                    std::string filename = "save.txt"; 
                    std::ifstream infile(filename);
                    std::vector<std::string> lines;
                    std::string line;
                    while (std::getline(infile, line)) {
                        lines.push_back(line);
                    }
                    infile.close();
                    std::string& fourth_line = lines[3];
                    std::stringstream ss1(fourth_line);
                    std::vector<std::string> values;
                    std::string value;
                    while (std::getline(ss1, value, ',')) {
                        values.push_back(value);
                    }
                    values[std::stoi(words[0])] = words[1];

                    std::stringstream new_line;
                    for (size_t i = 0; i < values.size(); i++) {
                        if (i > 0) {
                            new_line << ",";
                        }
                        new_line << values[i];
                    }
                    fourth_line = new_line.str();
                    std::ofstream outfile(filename);

                    for (const auto& l : lines) {
                        outfile << l << std::endl;
                    }
                    outfile.close();

                    /////////////////////////////
                    std::ifstream infile1(filename);
                    std::vector<std::string> lines1;
                    std::string line1;
                    while (std::getline(infile1, line1)) {
                        lines1.push_back(line1);
                    }
                    infile1.close();
                    std::stringstream bb;
                    for (size_t i = 0; i < 441; i++) {
                        if (i > 0) {
                            bb << " ";
                        }
                        bb << map[i];
                    }
                    lines1[6] = bb.str();
                    std::ofstream outfile1(filename);
                    for (const auto& l : lines1) {
                        outfile1 << l << std::endl;
                    }
                    outfile1.close();

                    return;
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b' && userInput.size() > 0)
                    {
                        userInput.pop_back();
                    }
                    else if (event.text.unicode != '\b')
                    {
                        userInput += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(userInput);
                }
            }
        }
        window.clear();
        window.draw(titulEnter);
        window.draw(txtEnter);
        window.draw(inputBox);
        window.draw(inputText);
        window.display();
    }
}
