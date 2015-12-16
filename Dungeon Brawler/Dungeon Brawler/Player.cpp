#include "Player.h"

Player::Player() 
	: mMeleeWeapon(0.5f, 100, false, Origin::FromPlayer)
	, mRangedWeapon(1, 150, true, ProjectileType::Arrow, Origin::FromPlayer)
	, mAnimTimer(0)
	, mInvincibilityTimer(0)
	, mGroundSensor(nullptr)
{}

void Player::init(TextureHolder& textureHolder, b2World &world, sf::Vector2f position)
{
	mDirection = Movement::Direction::Right;
	mJumpState = Movement::JumpState::OnGround;
//	mTargetVelX = SPEED;

	mAlive = true;
	mHit = false;

	mLives = 5;
	mScale = 0.5f;
	
	mVelocity = sf::Vector2f(0, 0);
	mSourcePos = sf::Vector2i(1, (int)AnimState::Idle);

	mSprite.setTexture(textureHolder.get(Textures::ID::Player));
	mSprite.setTextureRect(sf::IntRect((mSourcePos.x * 128) + (int)mDirection, mSourcePos.y * 256, 128, 256));
	mSprite.setScale(mScale, mScale);
	
	move(position);	//move the object, not the sprite

	mSize = sf::Vector2f(mSprite.getTextureRect().width * mScale, mSprite.getTextureRect().height * mScale);
	mOrigin = mSize / 2.f;
	setOrigin(mOrigin);

	setupBody(world, 1.f, 0.f);
	addGroundSensor();
	
	mMeleeWeapon.setupBody(world, 1, 0, getPosition());

	float y = getPosition().y;
	float x = getPosition().x;
	float width = getSize().x;
	float height = getSize().y;
	mRect = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, width, height);
}

void Player::setupBody(b2World &world, float density, float friction)
{
	const float scale = 30.f;

	//creating body
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2((getPosition().x) / scale, (getPosition().y) / scale);
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	mBody = world.CreateBody(&bodyDef);
	mBody->SetUserData(this);

	b2PolygonShape shape;
	shape.SetAsBox((mSize.x / 2) / scale, (mSize.y / 2) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.filter.groupIndex = -1;
	fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::Player;
	fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::PlayerMask;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}

void Player::addGroundSensor()
{
	const float scale = 30.f;

	b2PolygonShape shape;
	shape.SetAsBox((mSize.x / 3) / scale, 5 / scale, b2Vec2(0, (mSize.y / 2) / scale), 0);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::Player;
	fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::GroundOnly;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}



void Player::update(sf::Time dt, sf::Vector2f mousePos)
{ 
	if (mAlive)
	{
		faceDirection(mousePos);
		animateSprite(dt);
		hurt(dt);

		movement(dt);
		updateWeapons(dt);

		float y = getPosition().y;
		float x = getPosition().x;
		float width = getSize().x;
		float height = getSize().y;
		mRect = sf::FloatRect(x - width * 0.5f, y - height * 0.5f, width, height);
		std::shared_ptr<AudioManager> mAudioManager = AudioManager::sharedAudioManager();
		mAudioManager->set3Dlistenser(getPosition(), mVelocity);
	}
	else if (mBody->IsActive())
	{
		const float scale = 30.f;
		setPosition(sf::Vector2f(scale * mBody->GetPosition().x, scale * mBody->GetPosition().y));
		setRotation(mBody->GetAngle() * 180 / b2_pi);

		if (mJumpState == Movement::JumpState::OnGround)
			mBody->SetActive(false);
	}
}

void Player::faceDirection(sf::Vector2f mousePos)
{
	if (mousePos.x >= getPosition().x)
		mDirection = Movement::Direction::Right;
	else
		mDirection = Movement::Direction::Left;
}

void Player::updateWeapons(sf::Time dt)
{
	//UPDATE WEAPONS
	if (mDirection == Movement::Direction::Right)
		mMeleeWeapon.update(getPosition() + sf::Vector2f(mSize.x / 2.f, 0), false, dt);
	else
		mMeleeWeapon.update(getPosition() - sf::Vector2f(mSize.x / 2.f, 0), true, dt);

	mRangedWeapon.update(dt);
}

void Player::animateSprite(sf::Time dt)
{
	mAnimTimer += dt.asMilliseconds();

	if (mAnimTimer >= 1000 / (4*2))
	{
		mAnimTimer = 0;
		mSourcePos.x++;

		if (mSourcePos.x >= 5)
			mSourcePos.x = 1;
		
		mSprite.setTextureRect(sf::IntRect((mSourcePos.x * 128) + (int)mDirection, mSourcePos.y * 256, 128, 256));
	}
}

void Player::checkForNewState()
{
	if (mVelocity.y != 0)
		mSourcePos.y = (int)AnimState::Jump;
	else if (mVelocity.x != 0)
		mSourcePos.y = (int)AnimState::Move;
	else
		mSourcePos.y = (int)AnimState::Idle;
}

void Player::movement(sf::Time dt)
{//UPDATE MOVEMENT SO IT DOESNT GO THROUGH STUFF
	const float scale = 30.f;
	setPosition(sf::Vector2f(scale * mBody->GetPosition().x, scale * mBody->GetPosition().y));
	setRotation(mBody->GetAngle() * 180 / b2_pi);

//	lerpVelocity(mVelocity.x > mTargetVelX);
}

/*void Player::lerpVelocity(bool lerpDown)
{
	if (mVelocity.x != mTargetVelX)
	{
		if (lerpDown)
		{
			if (mVelocity.x > mTargetVelX)
			{
				mVelocity.x -= 0.25f;

				if (mVelocity.x < mTargetVelX)
				{
					mVelocity.x = mTargetVelX;
				}
			}
		}
		else
		{
			if (mVelocity.x < mTargetVelX)
			{
				mVelocity.x += 0.25f;
				if (mVelocity.x > mTargetVelX)
				{
					mVelocity.x = mTargetVelX;
				}
			}
		}
	}
}*/



bool Player::attackRanged() const
{//ATTACK BASED ON WHAT IS EQUIPED?
	bool weaponRanged = true;

	if (mMeleeWeapon.getEquiped())
		weaponRanged = false;
	
	return weaponRanged;
}

std::unique_ptr<Projectile> Player::createProjectile(sf::Vector2f target, sf::Vector2f offset, float pwrForce)
{
	sf::Vector2f spawnPosition = getPosition();

	if (mDirection == Movement::Direction::Right)
	{
		spawnPosition.x += mSize.x / 2;
		target += offset;
	}
	else
	{
		spawnPosition.x -= mSize.x / 2;
		target -= offset;
	}

	std::unique_ptr<Projectile> ptr(std::move(mRangedWeapon.createProjectile()));
	ptr->setUpArrow(target, pwrForce, spawnPosition);

	return ptr;
}

void Player::setAnimState(AnimState newAction, Movement::Direction direction)
{//SET UP ANIMATION STATE AND INPUTS
	if (mSourcePos.y != (int)newAction)
	{//if newAction isn't new then dont do it
		if (mJumpState == Movement::JumpState::OnGround)	//IF THE PLAYER IS ON THE GROUND THEN SET NEW ANIMATION STATE
		{
			mSourcePos.y = (int)newAction;
			mSourcePos.x = 1;
		}
	}

	if (newAction == AnimState::Idle)	//IF IDLE STOP MOVING
	{
		mVelocity.x = 0;
		//mTargetVelX = 0;
		mBody->SetLinearVelocity(b2Vec2(0, mBody->GetLinearVelocity().y));
	}
	else if (newAction == AnimState::Move)	
	{//IF MOVING THEN SET VELOCITY AND START/CONTINUE MOVING
		float speed = SPEED;
		if (mJumpState != Movement::JumpState::OnGround)
			speed /= 2;

		if (direction == Movement::Direction::Left)
			mVelocity.x = -speed;
		else
			mVelocity.x = speed;

		mBody->SetLinearVelocity(b2Vec2(mVelocity.x, mBody->GetLinearVelocity().y));

		//mTargetVelX = mVelocity.x;
	}
	else if (newAction == AnimState::Jump && mJumpState == Movement::JumpState::OnGround)
	{//IF JUMP IS PRESSED THEN CHECK IF NOT IN AIR, IF NOT THEN PREPARE TO JUMP
		mJumpState = Movement::JumpState::JumpUp;
		/*if (mDirection == Movement::Direction::Left)
			mTargetVelX = -SPEED / 2;
		else
			mTargetVelX = SPEED / 2;*/
		mVelocity.y = -SPEED;
		mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, mVelocity.y));
	}
}

void Player::switchWeapon()
{
	if (mMeleeWeapon.getEquiped())
	{
		mMeleeWeapon.setEquiped(false);
		mRangedWeapon.setEquiped(true);
	}
	else
	{
		mMeleeWeapon.setEquiped(true);
		mRangedWeapon.setEquiped(false);
	}
}

void Player::swingSword()
{
	if (mDirection == Movement::Direction::Right)
		mMeleeWeapon.attack(getPosition() + sf::Vector2f(mSize.x / 2.f, 0), false);
	else
		mMeleeWeapon.attack(getPosition() - sf::Vector2f(mSize.x / 2.f, 0), true);
}


void Player::hurt(sf::Time dt)
{
	const float INVICIBILITY_TIMER = 0.1f;

	if (mHit)
	{
		mInvincibilityTimer += dt.asSeconds();

		if (mInvincibilityTimer >= 1)
		{
			mHit = false;
			mInvincibilityTimer = 0;
		}
		else if (mInvincibilityTimer >= INVICIBILITY_TIMER)
		{
			mSprite.setColor(sf::Color::White);
		}
	}
}

void Player::hit(float damage)
{
	if (!mHit && mAlive)
	{
		mHit = true;
		mLives -= damage;
		mSprite.setColor(sf::Color::Red);

		if (mLives <= 0)
		{
			mSourcePos.y = (int)AnimState::MeleeAttack; //AnimState::Die;
			//rotate(90);
			//mBody->SetTransform(mBody->GetPosition(), getRotation() * acos(-1) / 180);
			mAlive = false;
		}
		auto mAudioManager = AudioManager::sharedAudioManager();
		mChannel = NULL;
		mAudioManager->playEffect(mAudioManager->mOuchSound, &mChannel);
	}
}

void Player::grounded(bool onGround, sf::FloatRect ground)
{
	if (onGround)
	{
		mGround = ground;
		mJumpState = Movement::JumpState::OnGround;
	}
	else
		mJumpState = Movement::JumpState::InAir;
}

void Player::landablePlatform(int index){
	b2Fixture* b2Fixture = mBody->GetFixtureList();
	b2Fixture = b2Fixture->GetNext();
	b2Filter b = b2Fixture->GetFilterData();
	b.groupIndex = index;
	b2Fixture->SetFilterData(b);
}

float Player::getLives() const
{
	return mLives;
}
bool Player::canFire() const
{
	return mRangedWeapon.canFire();
}
Movement::Direction	Player::getDirection() const
{
	return mDirection;
}
Movement::JumpState Player::getJumpState() const
{
	return mJumpState;
}
const sf::FloatRect & Player::getRect(){
	return mRect;
}