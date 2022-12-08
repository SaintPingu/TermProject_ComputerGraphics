#pragma once

enum class BGMSound { Normal = 0, _count };
enum class EffectSound { Fire = 0, Hit, Damaged, _count };
enum class SoundChannel { Bgm = 0, Effect, _count };

// has Channel
constexpr GLuint NUM_SOUND_BGM = static_cast<GLuint>(BGMSound::_count);
constexpr GLuint NUM_SOUND_EFFECT = static_cast<GLuint>(EffectSound::_count);
constexpr GLuint NUM_SOUND_CHEENEL = NUM_SOUND_BGM + NUM_SOUND_EFFECT;

// Stop�� Play�Ŀ� ����� ��
class SoundManager {
private:
	FMOD_SYSTEM* soundSystem = nullptr;
	FMOD_SOUND* bgmSoundList[NUM_SOUND_BGM]{};
	FMOD_SOUND* effectSoundList[NUM_SOUND_EFFECT]{};
	FMOD_CHANNEL* soundChannel[NUM_SOUND_CHEENEL]{};


public:
	SoundManager();

	GLvoid PlayEffectSound(const EffectSound& effectSound, const GLfloat& volume = 1.0f, GLboolean isNewChannel = false);
	GLvoid StopEffectSound();

	GLvoid PlayBGMSound(const BGMSound& bgmSound, const GLfloat& volume = 1.0f, GLboolean isNewChannel = false);
	GLvoid StopBGMSound();
};