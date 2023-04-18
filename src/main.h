#pragma once

#define WIDTH 21

#include <iostream>
#include <fstream>
#include "menu.h"
#include "game.h"
#include <sstream>

struct Options {
	bool sound = true;
	bool music = true;
	int level = 1;
	int players = 1;
	sf::Vector2i cols = sf::Vector2i(0, 0);
	int* external = nullptr;
	void getOpts() {
		std::ifstream file("save.txt");
		std::string line;
		getline(file, line);
		file.close();

		std::stringstream ss(line);
		int opts[3];
		for (int i = 0; i < 3; i++) {
			std::string temp;
			std::getline(ss, temp, ',');
			opts[i] = std::stoi(temp);
		}
		level = opts[0];
		sound = opts[1];
		music = opts[2];
	};
	void setOpts() {
		std::fstream file("save.txt", std::ios::in | std::ios::out);
		file.seekp(0);
		file << std::to_string(level) << "," << std::to_string(sound) << "," << std::to_string(music) << std::endl;
		file.close();
	}
};