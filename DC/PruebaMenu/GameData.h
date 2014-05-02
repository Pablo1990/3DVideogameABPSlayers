#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

class GameData
{
public:
	GameData(void);
	~GameData(void);
	bool save_game(int level, string name = "game01");
	int load_game(string name = "game01");
};

