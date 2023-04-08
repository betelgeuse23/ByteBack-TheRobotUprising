#include "Entity.h"

Entity::Entity(std::string texture, sf::Vector2i position) {
	this->position = position;
	this->texture.loadFromFile(texture);
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 31, 31));
	this->sprite.setPosition(sf::Vector2f(position * cell));
}

void Entity::initPatfind(const int* level, sf::Vector2i size, std::map<int, int> costs) {
	pf = PathFinder(level, size, costs);
}

void Entity::animate(State st, int fr, int sec) {
	this->sprite.setTextureRect(sf::IntRect(32 * ((fr * sec / cell) % sec), 32 * st, 31, 31));
}

void Entity::move(sf::Vector2i dest) {
	if(pf.isInit()) move(pf.pathfind(position, dest));
}

void Entity::move(Direction dir) {
	if (state == Still) {
		clock.restart();
		direction = dir;
		state = Moving;
	}
	else if (state == Moving && direction) {
		if (!(pf.isInit()) || (pf.isInit() && pf.isAccesible(direction, position))) {
			sf::Vector2i t_pos = position * cell;
			int time = clock.getElapsedTime().asMilliseconds();
			int change = std::min(int(time * speed), cell);

			t_pos += Utils::makeDir(direction) * change;

			animate(Moving, change, 4);
			sprite.setPosition(sf::Vector2f(t_pos));

			if (change == cell) {
				position += Utils::makeDir(direction);
				direction = None;
				state = Still;
			}
		}
		
	} else state = Still;
}



PathFinder::PathFinder(const int* level, sf::Vector2i size, std::map<int, int> costs)
	: size(size), accesible(std::vector<std::vector<int>>(size.x, std::vector<int>(size.y))), level(level), costs(costs), init(true) {
	makeCosts();
}

Direction PathFinder::pathfind(sf::Vector2i pos, sf::Vector2i dest) {
	if(!(init) || dest.x < 0 || dest.y < 0 || dest.x >= size.x || dest.y >= size.y || pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y) return Direction::None;

	makeCosts();

	struct Node {
		sf::Vector2i v;
		int c;
		int f;
		Node::Node(sf::Vector2i v, int c, int f) : v(v), c(c), f(f) {};
		inline bool operator==(const Node& oth) { return (this->v == oth.v) && (this->c == oth.c); }
	};

	auto compare = [](const Node n1, const Node n2) {return n1.f > n2.f; };
	auto distance = [](const sf::Vector2i p1, const sf::Vector2i p2) {return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)); };

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
	else Utils::vecCout(size, accesible);
}

bool PathFinder::isAccesible(const Direction dir, const sf::Vector2i pos) {
	sf::Vector2i posN = pos + Utils::makeDir(dir);
	if (!(init) || posN.x < 0 || posN.y < 0 || posN.x >= size.x || posN.y >= size.y || !(accesible[posN.x][posN.y])) return false;
	return true;
}

void PathFinder::makeCosts() {
	for (int i = 0, t = 0; i < size.x; i++) for (int j = 0; j < size.y; j++) accesible[j][i] = costs[level[t++]];
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
	return sf::Vector2i(roundf(pos.x / cell), roundf(pos.y / cell));
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