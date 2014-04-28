#pragma once
#include <irrKlang.h>
#include "Paths.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") 

class SoundEffect
{
public:
	SoundEffect(const char* bacground_path);
	~SoundEffect(void);
	void hit_sound();
	void win_sound();
	void lose_sound();
	void play_background();
	void stop_all_sounds();
private:
	ISoundEngine* klang_engine;
	ISoundSource* normal_hit;
	ISoundSource* bacground_music;
};

