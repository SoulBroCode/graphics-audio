#include "Enemy.h"


Enemy::Enemy() : mAnimTimer(0), mMove(false), Entity()
{}

void Enemy::init(TextureHolder& textureHolder, b2World &world, sf::Vector2f position)
{
	mDirection = Movement::Direction::Right;
	mJumpState = Movement::JumpState::InAir;

	mAlive = true;
	mHit = false;

	mLives = 1;
	mScale = 0.5f;

	mVelocity = sf::Vector2f(0, 0);
	//mSourcePos = sf::Vector2i(1, (int)AnimState::Idle);
	mState = State::Idle;
	mType = Type::Slime;

	mSprite.setTexture(textureHolder.get(Textures::ID::Slime));
	//	mSprite.setTextureRect(sf::IntRect((mSourcePos.x * 128) + (int)mDirection, mSourcePos.y * 256, 128, 256));
	mSprite.setScale(mScale, mScale);
	move(position);	//move the object, not the sprite

	mSize = sf::Vector2f(mSprite.getTextureRect().width * mScale, mSprite.getTextureRect().height * mScale);
	mOrigin = mSize / 2.f;
	setOrigin(mOrigin);

	mInvincibilityTimer = 0;

	setupBody(world, 1.f, 1.f);
	addGroundSensor();
	///////////////////////////audio//////////////////////////
	auto mAudioManager = AudioManager::sharedAudioManager();
	mChannel = NULL;
	mAudioManager->playEffect(mAudioManager->mSlimeSound, &mChannel);
	mChannel->set3DMinMaxDistance(80, 20000);
	mChannel->setVolume(0.6f);
	FMOD_VECTOR  sourcePos = { position.x, 0.0f, position.y };
	mChannel->set3DAttributes(&sourcePos, 0);
	///////////////////////////////////////////////////////////
}

void Enemy::setupBody(b2World &world, float density, float friction)
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
	fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::Ai;
	fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::AIMask;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}

void Enemy::addGroundSensor()
{
	const float scale = 30.f;

	b2PolygonShape shape;
	shape.SetAsBox((mSize.x / 2.5f) / scale, 2 / scale, b2Vec2(0, (mSize.y / 2) / scale), 0);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::Ai;
	fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::GroundOnly;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);

}


void Enemy::update(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	if (mAlive)
	{
		/////////////////////////////audio///////////////////////////////////
		FMOD_VECTOR  sourcePos = { getPosition().x, 0.0f, getPosition().y };
		mChannel->set3DAttributes(&sourcePos, 0);
		//////////////////////////////////////////////////////
		switch (mState)
		{
		case State::Idle:
			idle(dt);
			break;

		case State::Patrol:
			patrol(dt, playerPos, enemies);
			break;

		case State::Follow:
			follow(dt, playerPos, enemies);
			break;

		case State::Attack:
			attack(dt, playerPos, enemies);
			break;

		case State::Die:
			die(dt);
			break;

		default:
			break;
		}
	}
}

void Enemy::idle(sf::Time dt)
{
}
void Enemy::patrol(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
}
void Enemy::follow(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
}
void Enemy::attack(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
}
void Enemy::die(sf::Time dt)
{
}


void Enemy::canSeePlayer(sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	if (mGround.left != 0 && mGround.top != 0)
	{
		sf::Vector2f distV = playerPos - getPosition();
		float dist = sqrt((distV.x * distV.x) + (distV.y * distV.y));
		float lineOfSight = 500;

		if (mGround.width < 500)
			lineOfSight = mGround.width;


		if (dist <= lineOfSight)
		{
			if (playerPos.y < getPosition().y + (mGround.top - getPosition().y) + mGround.height / 2)
			{
				bool canSeePlayer = true;

				if (canSeePlayer)
				{
					if (playerPos.x <= mGround.left + mGround.width/2 && playerPos.x >= mGround.left)
						mState = State::Attack;
					else
						mState = State::Follow;
				}
				else
				{
					if (mState != State::Idle)
						mState = State::Patrol;
				}
			}
		}
		else
		{
			if (mState != State::Idle)
				mState = State::Patrol;
		}
	}
}


void Enemy::grounded(bool onGround, sf::FloatRect ground)
{
	if (onGround)
	{
		
		mGround = ground;
		mJumpState = Movement::JumpState::OnGround;
	}
	else{	
		mJumpState = Movement::JumpState::InAir;
		
	}
}


void Enemy::startContact()
{
	mContacting = true;
}
void Enemy::endContact()
{
	mContacting = false;
}

void Enemy::animateSprite(sf::Time dt)
{
	mAnimTimer += dt.asMilliseconds();

	if (mAnimTimer >= 1000 / (4 * 2))
	{
		mAnimTimer = 0;
		mSourcePos.x++;

		if (mSourcePos.x >= 5)
		{
			mSourcePos.x = 1;
		}

		mSprite.setTextureRect(sf::IntRect((mSourcePos.x * 128) + (int)mDirection, mSourcePos.y * 256, 128, 256));
	}
}

void Enemy::checkForNewState()
{
	if (mVelocity.y != 0)
	{
		mSourcePos.y = (int)AnimState::Jump;
	}
	else if (mVelocity.x != 0)
	{
		mSourcePos.y = (int)AnimState::Move;
	}
	else
	{
		mSourcePos.y = (int)AnimState::Idle;
	}
}

void Enemy::hurt(sf::Time dt)
{
	float INVICIBILITY_TIMER = 0.1f;

	if (mHit)
	{
		mInvincibilityTimer += dt.asSeconds();

		if (mInvincibilityTimer >= INVICIBILITY_TIMER)
		{
			mHit = false;
			mInvincibilityTimer = 0;
			mSprite.setColor(sf::Color::White);
		}
	}
}

void Enemy::hit(float damage)
{
	if (!mHit)
	{
		mHit = true;
		mLives -= damage;
		mSprite.setColor(sf::Color::Red);

		if (mLives <= 0){
			/////////////////////////////////audio//////////////////////
			auto mAudioManager = AudioManager::sharedAudioManager();
			FMOD::Channel *temp;
			mAudioManager->playEffect(mAudioManager->mArrowSound, &temp);
			temp->setVolume(0.3f);
			mChannel->setPaused(true);
			///////////////////////////////////////////////////////
			mState = State::Die;
		}
			
	}
}

void Enemy::sliced(MeleeWeapon* weapon)
{
	if (!mHit)
	{
		mHit = true;
		mLives -= weapon->getDamage();
		mSprite.setColor(sf::Color::Red);

		if (weapon->getPosition().x < getPosition().x)
			mBody->SetLinearVelocity(b2Vec2(SPEED/2, -SPEED / 4));
		else
			mBody->SetLinearVelocity(b2Vec2(-SPEED/2, -SPEED / 4));

		if (mLives <= 0){
			////////////////////////////audio///////////////////////////
			auto mAudioManager = AudioManager::sharedAudioManager();
			FMOD::Channel *temp;
			mAudioManager->playEffect(mAudioManager->mArrowSound, &temp);
			temp->setVolume(0.3f);
			mChannel->setPaused(true);
			///////////////////////////////////////////////////////
			mState = State::Die;
		}
			

	}
}

float Enemy::getLives() const
{
	return mLives;
}