#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include "Paths.h"
using namespace std;

class GameData
{
public:
	GameData(void);
	~GameData(void);
	bool save_game(int level, std::string name = "game01");
	int load_game(std::string name = "game01");
	bool save_config(float volume, int height, int widht, bool fullscreen);
	bool load_config(float &volume, int &height, int &widht, bool &fullscreen);
};

