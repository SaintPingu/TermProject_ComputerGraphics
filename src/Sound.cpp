#include "stdafx.h"
#include "Sound.h"


inline constexpr GLvoid CheckResult(const FMOD_RESULT& result)
{
	if (result != FMOD_OK)
	{
		assert(0);
	}
}
SoundManager::SoundManager()
{
	FMOD_System_Create(&soundSystem, FMOD_VERSION);
	FMOD_System_Init(soundSystem, 32, FMOD_INIT_NORMAL, NULL);

	FMOD_RESULT result = FMOD_OK;

	// bgmSound
	result = FMOD_System_CreateSound(soundSystem, "Sounds\\bgm_intro.wav", FMOD_LOOP_NORMAL, 0, &bgmSoundList[static_cast<GLint>(BGMSound::Normal)]);
	CheckResult(result);

	for (GLint i = 0; i < NUM_SOUND_CHEENEL; ++i)
	{
		soundChannel[i] = 0;
	}
}

GLvoid SoundManager::PlayEffectSound(const EffectSound& effectSound, const GLfloat& volume, GLboolean isNewChannel)
{
	FMOD_System_Update(soundSystem);
	if (isNewChannel == true)
	{
		FMOD_System_PlaySound(soundSystem, effectSoundList[static_cast<GLint>(effectSound)], 0, false, &soundChannel[static_cast<GLint>(SoundChannel::Effect)]);
		FMOD_Channel_SetVolume(soundChannel[static_cast<GLint>(SoundChannel::Effect)], volume);
	}
	else
	{
		FMOD_System_PlaySound(soundSystem, effectSoundList[static_cast<GLint>(effectSound)], 0, false, NULL);
	}
}
GLvoid SoundManager::StopEffectSound()
{
	FMOD_Channel_Stop(soundChannel[static_cast<GLint>(SoundChannel::Effect)]);
}

GLvoid SoundManager::PlayBGMSound(const BGMSound& bgmSound, const GLfloat& volume, GLboolean isNewChannel)
{
	FMOD_System_Update(soundSystem);
	if (isNewChannel == true)
	{
		FMOD_System_PlaySound(soundSystem, bgmSoundList[static_cast<GLint>(bgmSound)], 0, false, &soundChannel[static_cast<GLint>(SoundChannel::Bgm)]);
		FMOD_Channel_SetVolume(soundChannel[static_cast<GLint>(SoundChannel::Bgm)], volume);
	}
}
GLvoid SoundManager::StopBGMSound()
{
	FMOD_Channel_Stop(soundChannel[static_cast<GLint>(SoundChannel::Bgm)]);
}