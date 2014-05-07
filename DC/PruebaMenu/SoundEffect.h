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
	void pause_background_sounds();
	void resume_background_sounds();
	void set_volume(float volume);
	float get_volume();
private:
	ISoundEngine* klang_engine;
	ISoundSource* normal_hit;
	ISoundSource* background_music;
	ISound* background_sound;
};

