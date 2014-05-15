#include "SoundEffect.h"


SoundEffect::SoundEffect(const char* background_path)
{
	klang_engine = createIrrKlangDevice();

	background_music = klang_engine->getSoundSource(background_path, true);

	normal_hit = klang_engine->getSoundSource(hit_music_path);

	shield_hit = klang_engine->getSoundSource(shield_music_path);

	klang_engine->setSoundVolume(0.7f);
}


SoundEffect::~SoundEffect(void)
{

	if(klang_engine)
	{
		klang_engine->stopAllSounds();
		klang_engine->drop();
		klang_engine = 0;
	}

}

void SoundEffect::hit_sound()
{
	klang_engine->play2D(normal_hit);
}

void SoundEffect::win_sound()
{
	klang_engine->play2D(win_music_path);
}

void SoundEffect::lose_sound()
{
	klang_engine->play2D(lose_music_path);
}

void SoundEffect::shield_sound()
{
	klang_engine->play2D(shield_hit);
}

void SoundEffect::play_background()
{
	background_sound = klang_engine->play2D(background_music, true);
}

void SoundEffect::stop_all_sounds()
{
	klang_engine->stopAllSounds();
}

void SoundEffect::pause_background_sounds()
{
	if(background_sound)
		background_sound->setIsPaused(true);
}

void SoundEffect::resume_background_sounds()
{
	if(background_sound)
		background_sound->setIsPaused(false);
}

void SoundEffect::set_volume(float volume)
{
	klang_engine->setSoundVolume(volume);
}

float SoundEffect::get_volume()
{
	return klang_engine->getSoundVolume();
}