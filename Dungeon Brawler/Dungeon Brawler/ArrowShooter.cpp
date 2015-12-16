#include "ArrowShooter.h"

ArrowShooter::ArrowShooter() :
mRangedWeapon(0.375f, 150, true)
{

}


void ArrowShooter::init(sf::Texture const &texture, sf::Vector2f const &position, sf::IntRect const &spriteRect){

	mFireDelay = 1500;
	mSprite.setTexture(texture);
	mSprite.setTextureRect(spriteRect);
	mSprite.setPosition(position);
	mSprite.setOrigin(sf::Vector2f(spriteRect.width*0.5f, spriteRect.height*0.5f));
	mCanFire = true;
	mPauseSound = true;
	auto mAudioManager = AudioManager::sharedAudioManager();
	mChannel = NULL;
	mAudioManager->playEffect(mAudioManager->mGearSound, &mChannel);
	mChannel->setVolume(1.f);
	mChannel->set3DMinMaxDistance(50, 200);
	mChannel->setPaused(true);
}
void ArrowShooter::addBody(b2World &world){
	const float scale = 30.f;
	//creating body
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2((mSprite.getPosition().x) / scale, (mSprite.getPosition().y) / scale);
	BodyDef.type = b2_staticBody;
	BodyDef.fixedRotation = true;
	mBody = world.CreateBody(&BodyDef);
	mBody->SetUserData(this);
	b2PolygonShape Shape;
	Shape.SetAsBox((mSprite.getTextureRect().width / 2) / scale, (mSprite.getTextureRect().height / 2) / scale);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.0f;
	FixtureDef.shape = &Shape;
	mBody->CreateFixture(&FixtureDef);
	mBody->SetUserData("Wall");
}
void ArrowShooter::update(float dt){
	mFireDelay -= dt;
	if (mFireDelay < 0)
	{
		mFireDelay = 1500;
		mCanFire = true;
	}
}
void ArrowShooter::setFace(Face face){
	mArrowSpawnPos = mSprite.getPosition();
	float angle;
	float y = mSprite.getPosition().y;
	float x = mSprite.getPosition().x;
	float width = mSprite.getTextureRect().width;
	float height = mSprite.getTextureRect().height;
	switch (face){
	case Face::West:
	{
		mArrowSpawnPos.x -= mSprite.getTextureRect().width + 10;
		angle = 0;
		mLineOfSight = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, -(width * 9), height);
	}
	break;
	case Face::North:
		mArrowSpawnPos.y -= mSprite.getTextureRect().height + 10;
		angle = 90;
		mLineOfSight = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, width, -(height * 9));
		break;
	case Face::East:
		mArrowSpawnPos.x += mSprite.getTextureRect().width + 10;
		angle = 180;
		mLineOfSight = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, width * 9, height);
		break;
	default:
		mArrowSpawnPos.y += mSprite.getTextureRect().height + 10;
		angle = 270;
		mLineOfSight = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, width, height * 9);
		break;
	}
	mSprite.setRotation(angle);
}

void ArrowShooter::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	target.draw(mSprite, states);
}

void ArrowShooter::setTargetPos(sf::Vector2f const &pos){
	mTargetPos = pos;
}
sf::Vector2f const & ArrowShooter::getPos(){
	return mSprite.getPosition();
}

void ArrowShooter::setFireDelay(const float & delay){
	mFireDelay = delay;
}
const float & ArrowShooter::getFireDelay(){
	return mFireDelay;
}
void ArrowShooter::setPauseSound(bool canPlay){
	mPauseSound = canPlay;
	mChannel->setPaused(mPauseSound);
}
bool ArrowShooter::getCanFire(){
	return mCanFire;
}
void ArrowShooter::setCanFire(bool canFire){
	mCanFire = canFire;
}
bool ArrowShooter::getPauseSound(){
	return mPauseSound;
}

const sf::FloatRect & ArrowShooter::getLineOfSight(){
	return mLineOfSight;
}
std::unique_ptr<Projectile> ArrowShooter::createProjectile(float pwrForce)
{
	std::unique_ptr<Projectile> ptr(std::move(mRangedWeapon.createProjectile()));
	ptr->setUpArrow(mTargetPos, pwrForce, mArrowSpawnPos);
	return ptr;
}