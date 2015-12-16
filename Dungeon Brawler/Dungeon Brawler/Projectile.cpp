#include "Projectile.h"


Projectile::Projectile(float maxDamage, float range, ProjectileType type, Origin origin)
	: mSpeed(50), mDamage(maxDamage), mType(type), mOrigin(origin)
{}

void Projectile::setUpArrow(sf::Vector2f dir, float pwrForce, sf::Vector2f position)
{
	setPosition(position);
	calculateVelocity(pwrForce, dir);

	mDamage *= pwrForce;
}

void Projectile::setUpProjectile(TextureHolder& textures, b2World &world)
{
	if (mType == ProjectileType::Arrow)
	{
		mScale = 0.25f;

		mSprite.setTexture(textures.get(Textures::ID::Arrow));
		setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
		setScale(mScale, mScale);

		mSize = sf::Vector2f(mSprite.getTextureRect().width * mScale, mSprite.getTextureRect().height * mScale);
	}

	setupBody(world, 100.f, 1.f);
	addGroundSensor();
	mBody->SetLinearVelocity(b2Vec2(mVelocity.x, mVelocity.y));
}

void Projectile::setupBody(b2World &world, float density, float friction)
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
	shape.SetAsBox((mSize.x / 4) / scale, (mSize.y / 4) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	if (mOrigin == Origin::FromPlayer)
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::PlayerProjectile;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::PlayerMask;
	}
	else if (mOrigin == Origin::FromEnemy)
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::EnemyProjectile;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::AIMask;
	}
	else
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::TrapProjectile;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::TrapMask;
	}

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}

void Projectile::addGroundSensor()
{
	const float scale = 30.f;

	b2PolygonShape shape;
	shape.SetAsBox((mSize.x / 4) / scale, (mSize.y / 4) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::Player;
	fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::GroundOnly;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}

void Projectile::calculateVelocity(float pwrForce, sf::Vector2f posToGet)
{
	float speed = mSpeed * pwrForce;
	float uNum = 0;
	float angle = 0;
	
	sf::Vector2f dis = posToGet - getPosition();
	angle = atan2(dis.y, dis.x) * 180 / acos(-1);

	uNum = sqrt((dis.x * dis.x) + (dis.y * dis.y));
	dis /= uNum;

	mVelocity = dis * speed;
	rotate(angle);
}
void Projectile::calculateRotation()
{
	float angle = 0;
	angle = atan2(mVelocity.y, mVelocity.x) * 180 / acos(-1);

	setRotation(angle);
	mBody->SetTransform(mBody->GetPosition(), getRotation() * acos(-1) / 180);
}


void Projectile::newPosition(sf::Vector2f position)
{
	setPosition(position);
}


void Projectile::update(sf::Time dt)
{
	if (mBody->IsActive())
	{
		if (mHit)
		{
			mBody->SetActive(false);
			return;
		}

		const float GRAVITY = 9.81f;
		const float scale = 30.f;
		const sf::Vector2f OFFSET = sf::Vector2f(1.5f, 1.5f);

		sf::Vector2f newPos = sf::Vector2f(scale * mBody->GetPosition().x, scale * mBody->GetPosition().y);
		setPosition(newPos);

		mBody->SetLinearVelocity(b2Vec2(mVelocity.x, mVelocity.y));

		move(mVelocity / (float)dt.asMilliseconds());
		mVelocity.y += GRAVITY / (float)dt.asMilliseconds();

		calculateRotation();
	}
}

void Projectile::hit(float damage)
{
	mAlive = false;
	mHit = true;
}

void Projectile::grounded(bool onGround, sf::FloatRect ground)
{
	mAlive = false;
	mHit = true;
	auto mAudioManager = AudioManager::sharedAudioManager();
	FMOD::Channel *mChannel = NULL;
	mAudioManager->playEffect(mAudioManager->mArrowSound, &mChannel);
	mChannel->setVolume(1.f);
	mChannel->set3DMinMaxDistance(50, 2000);
	FMOD_VECTOR  sourcePos = { getPosition().x, 07.0f, getPosition().y };
	mChannel->set3DAttributes(&sourcePos, 0);
}

void Projectile::startContact()
{
	mContacting = true;
	mAlive = false;
	mHit = true;
}
void Projectile::endContact()
{
	mContacting = false;
	mAlive = false;
	mHit = true;
}

float Projectile::getDamage() const
{
	return mDamage;
}

const sf::Sprite& Projectile::getSprite() const
{
	return mSprite;
}