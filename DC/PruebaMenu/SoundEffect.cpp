#include "SoundEffect.h"


SoundEffect::SoundEffect(const char* background_path)
{
	klang_engine = createIrrKlangDevice();

	background_music = klang_engine->getSoundSource(background_path, true);

	normal_hit = klang_engine->getSoundSource(hit_music_path);
}


SoundEffect::~SoundEffect(void)
{

	
	if(background_music)
	{
		background_music->drop();
		background_music = 0;
	}

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

void SoundEffect::play_background()
{
	background_sound = klang_engine->play2D(background_music, true, false, true);

	if(background_sound)
	{
		background_sound->setVolume(0.5f);
	}
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