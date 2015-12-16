#include "Torch.h"

Torch::Torch()
{

}

void Torch::init(sf::Texture const &texture, sf::Vector2f const &position, sf::IntRect const &spriteRect){

	mSprite.setTexture(texture);
	mSprite.setTextureRect(spriteRect);
	mSprite.setPosition(position);
	mSprite.setScale(0.6f, 0.6f);
	mSprite.setOrigin(sf::Vector2f(spriteRect.width*0.5f, spriteRect.height*0.5f));
	
	auto mAudioManager = AudioManager::sharedAudioManager();
	mChannel = NULL;
	mAudioManager->playEffect(mAudioManager->mFireSound, &mChannel);

	mChannel->set3DMinMaxDistance(80, 20000);
	mChannel->setVolume(0.6f);
	FMOD_VECTOR  sourcePos = { position.x, 0.0f, position.y };
	mChannel->set3DAttributes(&sourcePos, 0);
	
}
void Torch::update(float dt){
	mAnimTimer += dt;
	

	if (mAnimTimer >= 100)
	{
		mAnimTimer = 0;
		mRowFrame++;
		if (mRowFrame >= 3)
		{
			mRowFrame = 0;
		}
		mSprite.setTextureRect(sf::IntRect(64 * mRowFrame, 0, 64, 200));
	}
}
void Torch::replaySound(){
	mChannel->stop();
	auto mAudioManager = AudioManager::sharedAudioManager();
	mAudioManager->playEffect(mAudioManager->mFireSound, &mChannel);
	if (mAudioManager->get3DAudio()){
		mChannel->set3DMinMaxDistance(80, 20000);
		mChannel->setVolume(0.6f);
		FMOD_VECTOR  sourcePos = { mSprite.getPosition().x, 0.0f, mSprite.getPosition().y };
		mChannel->set3DAttributes(&sourcePos, 0);
	}
}
void Torch::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	target.draw(mSprite, states);
}

