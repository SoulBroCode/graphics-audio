#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <iostream>
#include <memory>
#pragma comment(lib,"fmodex_vc.lib")
#include <fmod.hpp>
#include <fmod_errors.h>
#include <SFML\System\Vector2.hpp>
class AudioManager
{
public:
	enum Music{
		BG,
	};
	enum Effect{
		Arrow, Growl,
	};
	AudioManager();
	static std::shared_ptr<AudioManager> sharedAudioManager();
	void update();
	void createMusic(FMOD::Sound **pSound, const char* pFile, FMOD_MODE mode);
	void createEffect(FMOD::Sound **pSound, const char* pFile, FMOD_MODE mode);
	void playMusic(FMOD::Sound *pSound, FMOD::Channel **channel);
	void playEffect(FMOD::Sound *pSound, FMOD::Channel **channel);
	void set3Dlistenser(const sf::Vector2f &, const sf::Vector2f &);
	void setSourcePosition();	
	void changeEffectState();
	void changeMusicState();
	void change3DAudioState();
	void changeReverbState();
	void releaseSound(FMOD::Sound *pSound);
	bool getReverb();
	bool get3DAudio();
	FMOD::Sound *mBGSound;
	FMOD::Sound *mArrowSound;
	FMOD::Sound *mGearSound;
	FMOD::Sound *mFireSound;
	FMOD::Sound *mOuchSound;
	FMOD::Sound *mSlimeSound;
private:
	void reverbInit();
	static std::weak_ptr<AudioManager> mInstance;
	void init();
	FMOD::System *mSystem;
	FMOD::Reverb *mReverb;
	bool isMusicPause;
	bool isEffectPause;
	bool isReverb;
	bool is3DAudio;
	FMOD::ChannelGroup *mChannelMusic;
	FMOD::ChannelGroup *mChannelEffect;
};
#endif // GAME_H