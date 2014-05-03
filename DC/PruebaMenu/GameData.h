#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
using namespace std;

class GameData
{
public:
	GameData(void);
	~GameData(void);
	bool save_game(int level, std::string name = "game01");
	int load_game(std::string name = "game01");
};

