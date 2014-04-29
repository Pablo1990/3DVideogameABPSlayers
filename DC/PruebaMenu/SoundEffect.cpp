#include "SoundEffect.h"


SoundEffect::SoundEffect(const char* background_path)
{
	klang_engine = createIrrKlangDevice();

	bacground_music = klang_engine->getSoundSource(background_path, true);

	normal_hit = klang_engine->getSoundSource(hit_music_path);
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

void SoundEffect::play_background()
{
	ISound* snd = klang_engine->play2D(bacground_music);

	if(snd)
	{
		snd->setVolume(0.5f);
		snd->drop();
		snd = 0;
	}
}

void SoundEffect::stop_all_sounds()
{
	klang_engine->stopAllSounds();
}