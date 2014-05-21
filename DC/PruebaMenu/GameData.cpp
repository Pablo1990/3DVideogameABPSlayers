#include "GameData.h"


GameData::GameData(void)
{
}


GameData::~GameData(void)
{
}

bool GameData::save_game(int level, std::string name)
{
	ofstream save_file;
	try
	{
		stringstream ss;
		ss << "..\\save\\" << name << ".sav";
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

int GameData::load_game(std::string name)
{
	ifstream load_file;
	try
	{
		stringstream ss;
		ss << "..\\save\\" << name << ".sav";
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
			return 0;
		}
	}
	catch(...)
	{
		if(load_file.is_open())
		{
			load_file.close();
			return 0;
		}
	}

}

bool GameData::save_config(float volume, int height, int widht, bool fullscreen)
{
	ofstream save_file;
	try
	{


		
		save_file.open(config_path, ofstream::out | ofstream::trunc);

		if(save_file.is_open())
		{
			save_file << volume << endl;
			save_file << widht << "x" << height << endl;
			if(fullscreen)
				save_file << "1" << endl;
			else
				save_file << "0" << endl;
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

bool GameData::load_config(float &volume, int &height, int &widht, bool &fullscreen)
{
	ifstream load_file;
	try
	{
		load_file.open(config_path, ofstream::in);
		std::string screensize;
		std::string aux_height;
		std::string aux_width;
		std::string aux_volume;

		if(load_file.is_open())
		{
			load_file >> aux_volume;
			volume = atof(aux_volume.c_str());

			load_file >> screensize;
			
			stringstream ss(screensize);
			getline(ss, aux_height, 'x');
			getline(ss, aux_width, 'x');
			widht = atoi(aux_height.c_str());
			height = atoi(aux_width.c_str());

			load_file >> fullscreen;
			load_file.close();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(...)
	{
		if(load_file.is_open())
		{
			load_file.close();
			return false;
		}
	}

}
