#include "GameData.h"


GameData::GameData(void)
{
}


GameData::~GameData(void)
{
}

bool GameData::save_game(int level, string name)
{
	ofstream save_file;
	try
	{
		stringstream ss;
		ss << "../media/save/" << name << ".sav";
		string path = ss.str();

		
		save_file.open(path.c_str(), ofstream::out | ofstream::trunc);

		if(save_file.is_open())
		{
			save_file << level << endl;
			save_file.close();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(...)
	{
		if(save_file.is_open())
		{
			save_file.close();
			return false;
		}
	}
}

int GameData::load_game(string name)
{
	ifstream load_file;
	try
	{
		stringstream ss;
		ss << "../media/save/" << name << ".sav";
		string path = ss.str();
		string result;

		load_file.open(path.c_str(), ofstream::in);

		if(load_file.is_open())
		{
			load_file >> result;
			load_file.close();
			return atoi(result.c_str());
		}
		else
		{
			return 1;
		}
	}
	catch(...)
	{
		if(load_file.is_open())
		{
			load_file.close();
			return 1;
		}
	}

}