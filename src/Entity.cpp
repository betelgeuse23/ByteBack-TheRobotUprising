#include "Entity.h"

Entity::Entity(std::string texture, sf::Vector2i position) {
	this->position = position;
	this->texture.loadFromFile(texture);
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 31, 31));
	this->sprite.setPosition(sf::Vector2f(position * cell));
}

void Entity::initPatfind(Level* level, std::map<int, int> costs) {
	pf = PathFinder(level, this, costs);
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
	if (pf.isInit() && state == Still) {
		clock.restart();
		direction = pf.pathfind(dest);
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
	if(direction && (!(pf.isInit()) || (pf.isInit() && pf.isAccesible(direction, position)))) {
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



PathFinder::PathFinder(Level* level, Entity* that, std::map<int, int> costs)
	: level(level), that(that), accesible(std::vector<std::vector<int>>(level->size.x, std::vector<int>(level->size.y))), costs(costs), init(true) {
	makeCosts(false);
}

Direction PathFinder::pathfind(sf::Vector2i dest) {
	sf::Vector2i pos = that->getPosition(), size = level->size;
	if(!(init) || dest.x < 0 || dest.y < 0 || dest.x >= size.x || dest.y >= size.y || pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y) return Direction::None;

	makeCosts(true);
	
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

void PathFinder::info() {
	if (accesible.empty()) std::cout << "Empty" << std::endl;
	else Utils::vecCout(level->size, accesible);
}

bool PathFinder::isAccesible(const Direction dir, const sf::Vector2i pos) {
	if (!(init)) return false;

	makeCosts(false);
	sf::Vector2i posN = pos + Utils::makeDir(dir);

	if (posN.x < 0 || posN.y < 0 || posN.x >= level->size.x || posN.y >= level->size.y || !(accesible[posN.x][posN.y])) return false;
	return true;
}

bool PathFinder::doTrace(Direction dir, const sf::Vector2i pos, int range) {
	sf::Vector2i act_pos = pos;
	int r = 0;
	while (isAccesible(dir, act_pos) && range > r++) { act_pos += Utils::makeDir(dir); }
	act_pos += Utils::makeDir(dir);
	if (act_pos.x < 0 || act_pos.y < 0 || act_pos.x >= level->size.x || act_pos.y >= level->size.y) act_pos -= Utils::makeDir(dir);
	makeCosts(true);
	int ac = accesible[act_pos.x][act_pos.y];
	if (ac == 20 || ac == 2 || ac == 3 || ac == 25 || ac == 7 || ac == 8) return true;
	return false;
}

void PathFinder::makeCosts(bool search) {
	for (int i = 0, t = 0; i < level->size.x; i++) for (int j = 0; j < level->size.y; j++) accesible[j][i] = search?costs[level->map[t++]]:(costs[level->map[t++]]!= 1?0:1);
	if (!level->enemies.empty()) {
		for (auto& e : level->enemies) {
			if (e == that) continue;
			sf::Vector2i pos = e->getPosition(), size = level->size;
			Direction dir = e->getDirection();
			if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] = search ? 10 : 0;
			pos += Utils::makeDir(dir);
			if(pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] += search ? 5 : 0;
		}
	}
	if (search && !level->bullets.empty()) {
		for (auto& b : level->bullets) {
			sf::Vector2i pos = Utils::gPos(b->getPosition());
			accesible[pos.x][pos.y] = 100;
		}
	}
	if (level->player) {
		sf::Vector2i pos = level->player->getPosition(), size = level->size;
		Direction dir = level->player->getDirection();
		if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] = search ? 20 : 0;
		pos += Utils::makeDir(dir);
		if (pos.y >= 0 && pos.y < size.y && pos.x >= 0 && pos.x < size.x) accesible[pos.x][pos.y] += search ? 5 : 0;
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



bool Enemy::doDamage(int d) {
	health = std::max(0, health - d);
	if (health == 0) state = Dead;
	return state != Dead;
}

void Enemy::initStats(int h, int r, float s) {
	health = h;
	range = r;
	speed = s;
}

void Enemy::shoot(Level* level, Direction dir) {
	if (isCharged()) {
		level->bullets.push_back(new Bullet(position, dir, 1, false));
	}
}

bool Enemy::isCharged() {
	if (fire.getElapsedTime().asSeconds() > 1) { fire.restart(); return true; }
	else return false;
};

void Enemy::update() {
	Direction dir = pf.pathfind(pf.getLevel()->base);
	move(dir);
	if (pf.doTrace(dir, this->position, 10)) { shoot(pf.getLevel(), direction); }
	else if (pf.doTrace(Up, this->position, 10)) { shoot(pf.getLevel(), Up); }
	else if (pf.doTrace(Down, this->position, 10)) { shoot(pf.getLevel(), Down); }
	else if (pf.doTrace(Left, this->position, 10)) { shoot(pf.getLevel(), Left); }
	else if (pf.doTrace(Right, this->position, 10)) { shoot(pf.getLevel(), Right); }
	if (state == Moving) move();
}



bool Player::doDamage(int dmg) {
	lives = std::max(0, lives - dmg);
	setPosition(spawn);
	if (lives == 0) state = Dead;
	return state != Dead;
}

void Player::bonusSpeed() {
	clock.restart();
	damage = 1;
	speed += 0.1;
}

void Player::bonusLives() {
	lives++;
}

void Player::bonusDamage() {
	clock.restart();
	speed = 0.2;
	damage += 1;
}

void Player::update() {
	if (state == Moving) move();
	if ((damage > 1 || speed > 0.2) && clock.getElapsedTime().asSeconds() > 2) {
		damage = 1;
		speed = 0.2;
	}
}

bool Player::isCharged() {
	if (fire.getElapsedTime().asSeconds() > 1) { fire.restart(); return true; }
	else return false;
};

void Player::shoot(Level* level) {
	if (isCharged()) {
		level->bullets.push_back(new Bullet(position, lastDirection, damage, true));
	}
}