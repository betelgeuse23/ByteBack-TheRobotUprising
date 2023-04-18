#include "Entity.h"

Entity::Entity(std::string texture, sf::Vector2i position) {
	this->position = position;
	this->texture.loadFromFile(texture);
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 31, 31));
	this->sprite.setPosition(sf::Vector2f(position * cell));
}

void Entity::update() {
	if (state == Moving) move();
}

void Entity::setPosition(sf::Vector2i position) {
	this->position = position;
	state = Still;
	animate(Still, 0);
	sprite.setPosition(sf::Vector2f(position*cell));
}

void Entity::animate(State st, int fr) {
	int sec = animations[st] ? animations[st] : 1;
	this->sprite.setTextureRect(sf::IntRect(cell * ((fr * sec / cell) % sec), cell * st, cell - 1, cell - 1));
}

void Entity::move(sf::Vector2i dest) {
	if (pf && state == Still) {
		clock.restart();
		direction = pf->pathfind(position, dest);
		state = Moving;
	}
	if (direction) lastDirection = direction;
}

void Entity::move(Direction dir) {
	if (state == Still) {
		clock.restart();
		direction = dir;
		state = Moving;
	}
	if (direction) lastDirection = direction;
}

void Entity::move() {
	if(direction && (!(pf) || (pf && pf->isAccesible(direction, position)))) {
		sf::Vector2i t_pos = position * cell;
		int time = clock.getElapsedTime().asMilliseconds();
		int change = std::min(int(time * speed), cell);
		
		t_pos += Utils::makeDir(direction) * change;

		animate(Moving, change);
		sprite.setPosition(sf::Vector2f(t_pos));

		if (change == cell) {
			position += Utils::makeDir(direction);
			direction = None;
			state = Still;
		}
	}
	else {
		animate(Moving, 0);
		sprite.setPosition(sf::Vector2f(position * cell));
		state = Still;
	}
}



Direction PathFinder::pathfind(sf::Vector2i pos, sf::Vector2i dest) {
	sf::Vector2i size = level->size;
	if(dest.x < 0 || dest.y < 0 || dest.x >= size.x || dest.y >= size.y || pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y) return Direction::None;

	makeCosts(pos, true);
	
	struct Node {
		sf::Vector2i v;
		int c;
		int f;
		Node::Node(sf::Vector2i v, int c, int f) : v(v), c(c), f(f) {};
		inline bool operator==(const Node& oth) { return (this->v == oth.v) && (this->c == oth.c); }
	};

	auto compare = [](const Node n1, const Node n2) {return n1.f > n2.f; };
	auto distance = [](const sf::Vector2i p1, const sf::Vector2i p2) {return (int)std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)); };

	std::vector<Node> visited, opened;
	std::vector<std::vector<int>> costs(size.x, std::vector<int>(size.y, size.x * size.y + 10));

	costs[dest.x][dest.y] = 0;
	opened.push_back(Node(dest, 0, distance(dest, pos)));

	while (!opened.empty()) {
		std::sort(opened.begin(), opened.end(), compare);
		Node curr = opened.back();
		opened.pop_back();

		std::vector<Node> neigbors;
		if (curr.v.y > 0 && accesible[curr.v.x][curr.v.y - 1]) neigbors.push_back(Node(curr.v + Utils::makeDir(Up), costs[curr.v.x][curr.v.y - 1], costs[curr.v.x][curr.v.y - 1] + distance(curr.v + Utils::makeDir(Up), pos)));
		if (curr.v.x > 0 && accesible[curr.v.x - 1][curr.v.y]) neigbors.push_back(Node(curr.v + Utils::makeDir(Left), costs[curr.v.x - 1][curr.v.y], costs[curr.v.x - 1][curr.v.y] + distance(curr.v + Utils::makeDir(Left), pos)));
		if (curr.v.y < size.y - 1 && accesible[curr.v.x][curr.v.y + 1]) neigbors.push_back(Node(curr.v + Utils::makeDir(Down), costs[curr.v.x][curr.v.y + 1], costs[curr.v.x][curr.v.y + 1] + distance(curr.v + Utils::makeDir(Down), pos)));
		if (curr.v.x < size.x - 1 && accesible[curr.v.x + 1][curr.v.y]) neigbors.push_back(Node(curr.v + Utils::makeDir(Right), costs[curr.v.x + 1][curr.v.y], costs[curr.v.x + 1][curr.v.y] + distance(curr.v + Utils::makeDir(Right), pos)));
		std::sort(neigbors.begin(), neigbors.end(), compare);

		if (curr.v == pos) {
			Node t = neigbors.back();
			if (t.c < curr.c) {
				if (t.v.y < curr.v.y) return Direction::Up;
				if (t.v.y > curr.v.y) return Direction::Down;
				if (t.v.x < curr.v.x) return Direction::Left;
				if (t.v.x > curr.v.x) return Direction::Right;
			}
			return Direction::None;
		}

		visited.push_back(curr);

		for (auto nb : neigbors) {
			if (std::find(visited.begin(), visited.end(), nb) != visited.end()) continue;

			if (nb.c == size.x * size.y + 10 || nb.c > curr.c + 1) {
				costs[nb.v.x][nb.v.y] = curr.c + accesible[nb.v.x][nb.v.y];
				if (std::find(opened.begin(), opened.end(), nb) == opened.end()) opened.push_back(Node(nb.v, costs[nb.v.x][nb.v.y], costs[nb.v.x][nb.v.y] + distance(nb.v, pos)));
				else *std::find(opened.begin(), opened.end(), nb) = Node(nb.v, costs[nb.v.x][nb.v.y], costs[nb.v.x][nb.v.y] + distance(nb.v, pos));
			}
		}
	}

	return Direction::None;
}

bool PathFinder::isAccesible(const Direction dir, const sf::Vector2i pos) {
	makeCosts(pos, false);
	sf::Vector2i posN = pos + Utils::makeDir(dir);

	if (posN.x < 0 || posN.y < 0 || posN.x >= level->size.x || posN.y >= level->size.y || !(accesible[posN.x][posN.y])) return false;
	return true;
}

bool PathFinder::doTrace(Direction dir, const sf::Vector2i pos, int range, bool prime) {
	makeMatrix();
	sf::Vector2i act_pos = pos;
	int r = 0;
	while (isAccesible(dir, act_pos) && range > r++) { act_pos += Utils::makeDir(dir); }
	act_pos += Utils::makeDir(dir);
	if (act_pos.x < 0 || act_pos.y < 0 || act_pos.x >= level->size.x || act_pos.y >= level->size.y) act_pos -= Utils::makeDir(dir);
	int item = matrix[act_pos.x][act_pos.y];
	if (item == 20) return true;
	if (prime && (item == 1 || item == 2)) return true;
	return false;
}

void PathFinder::makeCosts(sf::Vector2i currPos, bool search) {
	for (int i = 0; i < level->size.x; i++) for (int j = 0; j < level->size.y; j++) accesible[j][i] = search?costs[level->map[j][i]]:(costs[level->map[j][i]]!= 1?0:1);
	if (!level->enemies.empty()) {
		for (auto& e : level->enemies) {
			sf::Vector2i pos = e->getPosition();
			if (pos == currPos) continue;
			sf::Vector2i size = level->size;
			Direction dir = e->getDirection();
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] = search ? 10 : 0;
			pos += Utils::makeDir(dir);
			if(pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] += search ? 5 : 0;
		}
	}
	if (!level->players.empty()) {
		for (auto& p : level->players) {
			sf::Vector2i pos = p->getPosition();
			if (pos == currPos) continue;
			sf::Vector2i size = level->size;
			Direction dir = p->getDirection();
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] = search ? 20 : 0;
			pos += Utils::makeDir(dir);
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] += search ? 5 : 0;
		}
	}
	if (search && !level->bullets.empty()) {
		for (auto& b : level->bullets) {
			sf::Vector2i pos = Utils::gPos(b->getPosition());
			accesible[pos.x][pos.y] = 100;
		}
	}
}

void PathFinder::makeAccess() {
	makeMatrix();
	for (int i = 0; i < level->size.x; i++) for (int j = 0; j < level->size.y; j++) accesible[j][i] = costs[matrix[j][i]] != 1 ? 0 : 1;
}

void PathFinder::makeMatrix() {
	for (int i = 0; i < level->size.x; i++) for (int j = 0; j < level->size.y; j++) matrix[j][i] = level->map[j][i];
	if (!level->enemies.empty()) {
		for (auto& e : level->enemies) {
			sf::Vector2i pos = e->getPosition(), size = level->size;
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) matrix[pos.x][pos.y] = 10;
		}
	}
	if (!level->players.empty()) {
		for (auto& p : level->players) {
			sf::Vector2i pos = p->getPosition(), size = level->size;
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) matrix[pos.x][pos.y] = 20;
		}
	}
}



sf::Vector2i Utils::makeDir(Direction dir) {
	switch (dir) {
	case Direction::Up: return sf::Vector2i(0, -1);
	case Direction::Down: return sf::Vector2i(0, 1);
	case Direction::Left: return sf::Vector2i(-1, 0);
	case Direction::Right: return sf::Vector2i(1, 0);
	default: return sf::Vector2i(0, 0);
	}
}

sf::Vector2i Utils::gPos(sf::Vector2f pos) {
	return sf::Vector2i((int)((pos.x + (int)(cell/2)) / cell), (int)((pos.y + (int)(cell / 2)) / cell));
}

void Utils::vecCout(sf::Vector2i size, std::vector<std::vector<int>> vec) {
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			std::cout.width(3);
			std::cout << vec[i][j] << " ";
		}
		std::cout << std::endl;
	} std::cout << std::endl;
}

int Utils::vPos(sf::Vector2f v) {
	return vPos(gPos(v));
}
int Utils::vPos(sf::Vector2i v) {
	return v.x + v.y * WIDTH;
}



void Mpl::playShoot() {
	sb.loadFromFile("sounds/laserShoot.wav");
	s = sf::Sound(sb);
	if(!offed) s.play();
}

void Mpl::playHit() {
	sb.loadFromFile("sounds/hitHurt.wav");
	s = sf::Sound(sb);
	if (!offed) s.play();
}

void Mpl::playGet() {
	sb.loadFromFile("sounds/powerUp.wav");
	s = sf::Sound(sb);
	if (!offed) s.play();
}

void Mpl::playLoose() {
	sb.loadFromFile("sounds/loose.wav");
	s = sf::Sound(sb);
	if (!offed) s.play();
}

void Mpl::playWin() {
	sb.loadFromFile("sounds/win.wav");
	s = sf::Sound(sb);
	if (!offed) s.play();
}



bool Enemy::doDamage(int d) {
	health = std::max(0, health - d);
	if (health == 0) {
		state = Dead;
		int b = rand() % 8;
		if(b < 5) pf->getLevel()->bonuses.push_back(new Bonus(Effects(b), position));
	}
	return state != Dead;
}

void Enemy::initStats(int h, int r, int d, float s) {
	health = h;
	range = r;
	damage = d;
	speed = s;
}

void Enemy::shoot(Level* level, Direction dir) {
	if (isCharged()) {
		level->bullets.push_back(new Bullet(position, dir, damage, false));
		level->pl.playShoot();
	}
}

bool Enemy::isCharged() {
	if (fire.getElapsedTime().asSeconds() > 1) { fire.restart(); return true; }
	else return false;
};

void Enemy::update() {
	Direction dir = pf->pathfind(position, target);
	if (state == Moving) move();
	if (state != Moving) {
		if (direction != Up && pf->doTrace(Up, this->position, 10, false)) { shoot(pf->getLevel(), Up); }
		else if (direction != Down && pf->doTrace(Down, this->position, 10, false)) { shoot(pf->getLevel(), Down); }
		else if (direction != Left && pf->doTrace(Left, this->position, 10, false)) { shoot(pf->getLevel(), Left); }
		else if (direction != Right && pf->doTrace(Right, this->position, 10, false)) { shoot(pf->getLevel(), Right); }
		else if (direction != None && pf->doTrace(direction, this->position, 10, true)) { shoot(pf->getLevel(), direction); }
	}
	move(dir);
}



bool Player::doDamage(int dmg) {
	if (dmg == -1) affect(Stan);
	if (dmg == -5) lives = 0;

	if (effect != Shield && dmg > 0) {
		lives = std::max(0, lives - 1);
		setPosition(spawn);
	}
	
	if (lives == 0) state = Dead;
	return state != Dead;
}

void Player::affect(Effects eff) {
	effect = eff;
	bonus.restart();

	if (eff != Spare && eff != Stan) pf->getLevel()->pl.playGet();

	if(eff != Speed) speed = 0.2;
	else if (speed < 0.4) speed += 0.1;

	if(eff != Damage) damage = 1;
	else if (damage < 3) damage += 1;

	if(eff != Rate) rate = 1000;
	else if (rate > 100) rate -= 300;

	if (eff == Health) {
		if(lives < 5) lives++;
		effect = Spare;
	}

	if(eff == Stan) speed = 0;
}

void Player::update() {
	if (state == Moving) move();
	if (effect != Spare && bonus.getElapsedTime().asSeconds() > 4) affect(Spare);
}

bool Player::isCharged() {
	if (fire.getElapsedTime().asMilliseconds() > rate) { fire.restart(); return true; }
	else return false;
};

void Player::shoot(Level* level) {
	if (isCharged()) {
		level->bullets.push_back(new Bullet(sprite.getPosition(), lastDirection, damage, true));
		level->pl.playShoot();
	}
}



void Spawner::spawn(int* map) {
	sf::Vector2i size = level->size;
	for (int i = 0, t = 0, p = 0; i < size.x; i++) for (int j = 0; j < size.y; j++, t++) {
		switch (map[t]) {
		case 11:
			spawnRobot1(sf::Vector2i(j, i));
			break;
		case 12:
			spawnRobot2(sf::Vector2i(j, i));
			break;
		case 13:
			spawnRobot3(sf::Vector2i(j, i));
			break;
		case 14:
			spawnRobot4(sf::Vector2i(j, i));
			break;
		case 15:
			spawnRobot5(sf::Vector2i(j, i));
			break;
		case 20:
			if (colors != sf::Vector2i(0, 0) && p < 2) spawnPlayer(p==0?colors.x:colors.y, sf::Vector2i(j, i));
			if(colors == sf::Vector2i(0, 0) && p < 1) spawnPlayer(std::rand()%4, sf::Vector2i(j, i));
			p++;
			break;
		}
	}
}

void Spawner::spawnPlayer(int col, sf::Vector2i pos) {
	std::string str;
	switch (col) {
	case 0:
		str = "images/player1.png";
		break;
	case 1:
		str = "images/player2.png";
		break;
	case 2:
		str = "images/player3.png";
		break;
	case 3:
		str = "images/player4.png";
		break;
	}
	Player* pl = new Player(str, pos, col);
	pl->initPatfind(&pfP);
	pl->initAnimation({ {Moving, 2} });
	level->players.push_back(pl);
}

void Spawner::spawnRobot1(sf::Vector2i pos) {
	Enemy* en = new Enemy("images/robot1.png", pos);
	en->initPatfind(&pfE);
	en->makeTarget();
	en->initAnimation({ {Moving, 4} });
	en->initStats(1, 5, 1, (float)0.1);
	level->enemies.push_back(en);
}

void Spawner::spawnRobot2(sf::Vector2i pos) {
	Enemy* en = new Enemy("images/robot2.png", pos);
	en->initPatfind(&pfEB);
	en->makeTarget();
	en->initAnimation({ {Moving, 4} });
	en->initStats(1, 15, 1, (float)0.1);
	level->enemies.push_back(en);
}

void Spawner::spawnRobot3(sf::Vector2i pos) {
	Enemy* en = new Enemy("images/robot3.png", pos);
	en->initPatfind(&pfE);
	en->makeTarget();
	en->initAnimation({ {Moving, 4} });
	en->initStats(2, 7, 1, (float)0.05);
	level->enemies.push_back(en);
}

void Spawner::spawnRobot4(sf::Vector2i pos) {
	Enemy* en = new Enemy("images/robot4.png", pos);
	en->initPatfind(&pfEA);
	en->makeTarget();
	en->initAnimation({ {Moving, 8} });
	en->initStats(1, 20, -1, (float)0.15);
	level->enemies.push_back(en);
}

void Spawner::spawnRobot5(sf::Vector2i pos) {
	Enemy* en = new Enemy("images/robot5.png", pos);
	en->initPatfind(&pfE);
	en->makeTarget();
	en->initAnimation({ {Moving, 2} });
	en->initStats(1, 7, 3, (float)0.1);
	level->enemies.push_back(en);
}