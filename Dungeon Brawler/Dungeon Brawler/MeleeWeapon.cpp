#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(float damage, float range, bool equiped, Origin origin)
	:Weapon(damage, range, equiped), mOrigin(origin)
{}

void MeleeWeapon::setupBody(b2World &world, float density, float friction, sf::Vector2f positon)
{
	const float scale = 30.f;

	//creating body
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(positon.x / scale, positon.y / scale);
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	mBody = world.CreateBody(&bodyDef);
	mBody->SetUserData(this);

	b2PolygonShape shape;
	shape.SetAsBox((3 * mRange / 4) / scale, (mRange / 2) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	if (mOrigin == Origin::FromPlayer)
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::PlayerMelee;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::PlayerMeleeMask;
	}
	else if (mOrigin == Origin::FromEnemy)
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::EnemyMelee;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::AIMeleeMask;
	}
	else
	{
		fixtureDef.filter.categoryBits = (uint16)Colliders::ColliderTags::TrapMelee;
		fixtureDef.filter.maskBits = (uint16)Colliders::ColliderMasks::TrapMeleeMask;
	}

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
	mBody->SetGravityScale(0);
	mBody->SetActive(false);
}

void MeleeWeapon::attack(sf::Vector2f position, bool left)
{
	const float scale = 30.f;

	if (left)
		mBody->SetTransform(b2Vec2((position.x - (mRange / 2)) / scale, position.y / 30), 0);
	else
		mBody->SetTransform(b2Vec2((position.x + (mRange / 2)) / scale, position.y / 30), 0);

	mBody->SetActive(true);
}

void MeleeWeapon::notAttacking()
{
	mBody->SetActive(false);
}

void MeleeWeapon::levelUp()
{
	mMasteryLevel++;
}

void MeleeWeapon::update(sf::Vector2f position, bool left, sf::Time dt)
{
	if (mBody->IsActive())
	{
		const float scale = 30.f;

		if (left)
			mBody->SetTransform(b2Vec2((position.x - (mRange / 4)) / scale, position.y / 30), 0);
		else
			mBody->SetTransform(b2Vec2((position.x + (mRange / 4)) / scale, position.y / 30), 0);

		timer(dt);
	}
}

void MeleeWeapon::timer(sf::Time dt)
{
	mAttackTimer += dt.asSeconds();

	if (mAttackTimer >= MAX_TIMER)
	{
		notAttacking();
		mAttackTimer = 0;
	}
}

sf::Vector2f MeleeWeapon::getPosition()
{
	return sf::Vector2f(mBody->GetPosition().x *30, mBody->GetPosition().y *30);
}