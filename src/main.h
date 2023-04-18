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
	void getOpts() {
		std::ifstream file("save.txt");
		std::string line;
		getline(file, line);
		file.close();

		std::stringstream ss(line);
		int opts[4];
		for (int i = 0; i < 4; i++) {
			std::string temp;
			std::getline(ss, temp, ',');
			opts[i] = std::stoi(temp);
		}
		level = opts[0];
		players = opts[1];
		sound = opts[2];
		music = opts[3];
	};
	void setOpts() {
		std::fstream file("save.txt", std::ios::in | std::ios::out);
		file.seekp(0);
		file << std::to_string(level) << "," << std::to_string(players) << "," << std::to_string(sound) << "," << std::to_string(music) << std::endl;
		file.close();
	}
};