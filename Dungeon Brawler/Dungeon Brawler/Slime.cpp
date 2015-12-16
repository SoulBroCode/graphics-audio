#include "Slime.h"


Slime::Slime() : mMeleeWeapon(0.5f, 30, false, Origin::FromEnemy)
{}

void Slime::init(TextureHolder& textureHolder, b2World &world, sf::Vector2f position)
{
	Enemy::init(textureHolder, world, position);
}


void Slime::update(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	Enemy::update(dt, playerPos, enemies);

	if (mAlive)
	{
		//	animateSprite(dt);
		hurt(dt);

		const float scale = 30.f;
		setPosition(sf::Vector2f(scale * mBody->GetPosition().x, scale * mBody->GetPosition().y));
		setRotation(mBody->GetAngle() * 180 / b2_pi);

		canSeePlayer(playerPos, enemies);
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


void Slime::bounce()
{
	if (mJumpState == Movement::JumpState::OnGround)
	{
		mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x, -SPEED/5));
		mJumpState = Movement::JumpState::JumpUp;
	}
}

void Slime::idle(sf::Time dt)
{
	//Enemy::idle(dt);
	bounce();
}

void Slime::patrol(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	//Enemy::patrol(dt, playerPos, enemies);
	bounce();
}

void Slime::follow(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	//Enemy::follow(dt, playerPos, enemies);

	if (getPosition().x + (mSize.x / 2) <= mGround.left + mGround.width / 2 && getPosition().x - (mSize.x / 2) >= mGround.left)
	{
		if (playerPos.x - getPosition().x > 0)
			mVelocity.x = SPEED / 2.25f;
		else
			mVelocity.x = -SPEED / 2.25f;

		mBody->SetLinearVelocity(b2Vec2(mVelocity.x, mBody->GetLinearVelocity().y));
	}
	else
	{
		if (getPosition().x + (mSize.x/2) < mGround.left + mGround.width/2)
			setPosition(sf::Vector2f(mGround.left + mGround.width/2, getPosition().y));
		else if (getPosition().x - (mSize.x / 2) > mGround.left)
			setPosition(sf::Vector2f(mGround.left, getPosition().y));

		mVelocity.x = 0;
		mBody->SetLinearVelocity(b2Vec2(mVelocity.x, mBody->GetLinearVelocity().y));
	}

	bounce();
}

void Slime::attack(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	/*if (playerPos.x - getPosition().x > 0)
		mVelocity.x = SPEED / 2.25;
	else
		mVelocity.x = -SPEED/2.25f;*/

	//bounce();
}

void Slime::die(sf::Time dt)
{
	rotate(90);
	mAlive = false;
}