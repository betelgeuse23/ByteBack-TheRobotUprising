#include "Entity.h"

void log_i(std::string s) {
	std::cout << s << std::endl;
}

Entity::Entity(std::string texture, sf::Vector2f position) {
	this->position = position;
	this->texture.loadFromFile(texture);
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect(0, 0, 31, 31));
	this->sprite.setPosition(position);
}

void Entity::draw(sf::RenderWindow& window) { window.draw(sprite); }

void Entity::animate(int ch, int sec) {
	this->sprite.setTextureRect(sf::IntRect(32 * (int(ch * sec / cell) % sec), 0, 31, 31));
}

void Entity::move(Direction dir) {
	if (this->state == Still) {
		this->clock.restart();
		this->direction = dir;
		this->state = Moving;
	}
}

void Entity::move() {
	if (this->direction) {
		sf::Vector2f t_pos = this->position;
		int time = this->clock.getElapsedTime().asMilliseconds();
		int change = std::min(int(time * speed), cell);
		
		if (this->direction == 1)
			t_pos.y -= change;
		else if (this->direction == 2)
			t_pos.y += change;
		else if (this->direction == 3)
			t_pos.x -= change;
		else if (this->direction == 4)
			t_pos.x += change;

		this->animate(change, 4);
		this->sprite.setPosition(t_pos);

		if (change == cell) {
			this->position = t_pos;
			this->direction = None;
			this->state = Still;
		}
	}
	else this->state = Still;
}

void Entity::info() {
	log_i(std::to_string(this->state) + " " + std::to_string(this->direction));
}

Direction pathfind(const int* level, sf::Vector2f size, sf::Vector2f pos, sf::Vector2f dest) {
	struct Node {
		sf::Vector2f v;
		int c;
		int f;
		Node::Node(sf::Vector2f v, int c, int f) : v(v), c(c), f(f) {};
		inline bool operator==(const Node& oth) { return (this->v == oth.v) && (this->c == oth.c); }
	};
	
	auto compare = [](Node n1, Node n2) {return n1.c > n2.c; };
	auto distance = [](sf::Vector2f p1, sf::Vector2f p2) {return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)); };

	std::vector<Node> visited, opened;
	std::vector<std::vector<int>> costs(size.x, std::vector<int>(size.y, size.x* size.y + 10));
	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < size.y; j++)
			if (level[i  + j * int(size.x)] != 0)
				costs[i][j] = -1;

	if(	dest.x < 0 || dest.y < 0 || dest.x >= size.x || dest.y >= size.y ||
		pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y ||
		costs[dest.x][dest.y] == -1) return Direction::None;
	costs[dest.x][dest.y] = 0;
	opened.push_back(Node(dest, 0, distance(dest, pos)));

	while (!opened.empty()) {
		std::sort(opened.begin(), opened.end(), compare);
		Node curr = opened.back();
		opened.pop_back();

		std::vector<Node> neigbors;
		if (curr.v.y > 0 && costs[curr.v.x][curr.v.y - 1] != -1) neigbors.push_back(Node(curr.v - sf::Vector2f(0, 1), costs[curr.v.x][curr.v.y - 1], costs[curr.v.x][curr.v.y - 1] + distance(curr.v - sf::Vector2f(0, 1), pos)));
		if (curr.v.x > 0 && costs[curr.v.x - 1][curr.v.y] != -1) neigbors.push_back(Node(curr.v - sf::Vector2f(1, 0), costs[curr.v.x - 1][curr.v.y], costs[curr.v.x - 1][curr.v.y] + distance(curr.v - sf::Vector2f(1, 0), pos)));
		if (curr.v.y < size.y - 1 && costs[curr.v.x][curr.v.y + 1] != -1) neigbors.push_back(Node(curr.v + sf::Vector2f(0, 1), costs[curr.v.x][curr.v.y + 1], costs[curr.v.x][curr.v.y + 1] + distance(curr.v + sf::Vector2f(0, 1), pos)));
		if (curr.v.x < size.x - 1 && costs[curr.v.x + 1][curr.v.y] != -1) neigbors.push_back(Node(curr.v + sf::Vector2f(1, 0), costs[curr.v.x + 1][curr.v.y], costs[curr.v.x + 1][curr.v.y] + distance(curr.v + sf::Vector2f(1, 0), pos)));
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
				costs[nb.v.x][nb.v.y] = curr.c + 1;
				if (std::find(opened.begin(), opened.end(), nb) == opened.end()) opened.push_back(Node(nb.v, curr.c + 1, curr.c + 1 + distance(nb.v, pos)));
				else *std::find(opened.begin(), opened.end(), nb) = Node(nb.v, curr.c + 1, curr.c + 1 + distance(nb.v, pos));
			}
		}
	}

	return Direction::None;
}

sf::Vector2f gPos(sf::Vector2f pos){
	pos.x = roundf(pos.x / cell);
	pos.y = roundf(pos.y / cell);
	return pos;
}