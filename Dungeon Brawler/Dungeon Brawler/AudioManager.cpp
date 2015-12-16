#include "AudioManager.h"
std::weak_ptr<AudioManager> AudioManager::mInstance;

AudioManager::AudioManager(){}

std::shared_ptr<AudioManager> AudioManager::sharedAudioManager()
{
	std::shared_ptr<AudioManager> shareObject = mInstance.lock();
	if (!shareObject)
	{
		shareObject = std::make_shared<AudioManager>(AudioManager());
		shareObject->init();
		mInstance = shareObject;	
	}
	return shareObject;
}

void AudioManager::init(){
	//FMOD_RESULT result;

	isMusicPause = false;
	isEffectPause = false;
	isReverb = true;
	is3DAudio = true;
	if (FMOD::System_Create(&mSystem) != FMOD_OK){		
		//std::cout << "FMOD error!" << result << FMOD_ErrorString(result);
		std::cout << "Create System Error! (%d) %s\n";
		exit(-1);
	}
	if (mSystem->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK){
		std::cout << "System Init Error! (%d) %s\n";
		exit(-1);
	}
	

	createMusic(&mBGSound, "Media/Sounds/dungeon.mp3", FMOD_DEFAULT | FMOD_LOOP_NORMAL);
	mBGSound->setLoopCount(-1);
	createEffect(&mArrowSound, "Media/Sounds/hit.wav", FMOD_3D);
	createEffect(&mGearSound, "Media/Sounds/gear.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL);
	mGearSound->setLoopCount(-1);
	createEffect(&mFireSound, "Media/Sounds/fire.wav", FMOD_3D | FMOD_LOOP_NORMAL);
	mFireSound->setLoopCount(-1);
	FMOD_VECTOR curve[3] = { { 0.0f, 1.0f, 0.0f }, { 0.2f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
	mFireSound->set3DCustomRolloff(curve, 0);
	createEffect(&mOuchSound, "Media/Sounds/ouch.wav", FMOD_DEFAULT);
	createEffect(&mSlimeSound, "Media/Sounds/slime.wav", FMOD_3D | FMOD_LOOP_NORMAL);
	mSlimeSound->set3DCustomRolloff(curve, 0);

	mSystem->createChannelGroup("ChannelGroupMusic", &mChannelMusic);
	mSystem->createChannelGroup("ChannelGroupEffect", &mChannelEffect);
	reverbInit();
	mReverb->setActive(isReverb);
}
void AudioManager::update(){
	mSystem->update();
}
void AudioManager::createMusic(FMOD::Sound **pSound, const char* pFile, FMOD_MODE mode)
{
	if (mSystem->createStream(pFile, mode, 0, pSound) != FMOD_OK)
	{
		std::cout << "Create Sound Error! (%d) %s\n";
		exit(-1);
	}

}

void AudioManager::playMusic(FMOD::Sound *pSound, FMOD::Channel **channel){
	if (mSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, channel) != FMOD_OK)
	{
		std::cout << "Load Sound Error! (%d) %s\n";
		exit(-1);
	}
	(*channel)->setChannelGroup(mChannelMusic);
}

void AudioManager::createEffect(FMOD::Sound **pSound, const char* pFile, FMOD_MODE mode)
{
	if (mSystem->createSound(pFile, mode, 0, pSound) != FMOD_OK)
	{
		std::cout << "Create Sound Error! (%d) %s\n";
		exit(-1);
	}
}

void AudioManager::playEffect(FMOD::Sound *pSound, FMOD::Channel **channel){

	if (mSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, channel) != FMOD_OK)
	{
		std::cout << "Load Sound Error! (%d) %s\n";
		exit(-1);
	}

	(*channel)->setChannelGroup(mChannelEffect);
	//mChannelEffect->setPaused(true);
}
void AudioManager::set3Dlistenser(const sf::Vector2f &position, const sf::Vector2f &velocity){
	FMOD_VECTOR  listenervel = { velocity.x, 0.0f, velocity.y };
	FMOD_VECTOR  listenerpos = { position.x, 0.0f, position.y };
	mSystem->set3DListenerAttributes(0, &listenerpos, &listenervel, 0, 0);
}
void AudioManager::reverbInit(){
	mSystem->createReverb(&mReverb);
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_CAVE;

	mReverb->setProperties(&prop);

	FMOD_VECTOR pos = {2500, 0.0f,350 };
	float mindist = 400;
	float maxdist = 500;
	mReverb->set3DAttributes(&pos, mindist, maxdist);
}

void AudioManager::releaseSound(FMOD::Sound *pSound){
	pSound->release();
}

void AudioManager::changeEffectState(){
	if (isEffectPause){
		isEffectPause = false;
	}
	else{
		isEffectPause = true;
	}	
	mChannelEffect->setPaused(isEffectPause);
}
void AudioManager::changeMusicState(){
	if (isMusicPause){
		isMusicPause = false;
	}
	else{
		isMusicPause = true;	
	}
	mChannelMusic->setPaused(isMusicPause);
}
void AudioManager::change3DAudioState(){
	if (is3DAudio){
		is3DAudio = false;
		mArrowSound->setMode(FMOD_2D);
		mFireSound->setMode(FMOD_2D); 
	}
	else{
		is3DAudio = true;
		mArrowSound->setMode(FMOD_3D);
		mFireSound->setMode(FMOD_3D);
	}
	
}
void AudioManager::changeReverbState(){
	if (isReverb){
		isReverb = false;
	}
	else{
		isReverb = true;
	}
	mReverb->setActive(isReverb);
}

bool AudioManager::getReverb(){
	return isReverb;
}
bool  AudioManager::get3DAudio(){
	return is3DAudio;
}