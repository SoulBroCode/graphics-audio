#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "RangeWeapon.h"
#include "MeleeWeapon.h"
#include "AudioManager.h"
#include <memory.h>

class Player : public Entity
{
public:
	enum class AnimState 
	{
		Idle,
		Move,
		MeleeAttack,
		RangeAttack,
		Jump,
		Die
	};

public:
									Player();
	void							init(TextureHolder& textureHolder,b2World &world, sf::Vector2f position);

	void							update(sf::Time dt, sf::Vector2f mousePos);

	virtual void					hit(float damage) override;
	virtual void					grounded(bool onGround, sf::FloatRect ground) override;
	virtual void					landablePlatform(int index) override;

	void							setAnimState(AnimState newAction, Movement::Direction direction);
	void							switchWeapon();
	void							swingSword();

	std::unique_ptr<Projectile>		createProjectile(sf::Vector2f dir, sf::Vector2f offset, float pwrForce);

	float							getLives() const;
	bool							attackRanged() const;
	bool							canFire() const;

	Movement::Direction				getDirection() const;
	Movement::JumpState				getJumpState() const;
	const sf::FloatRect &			getRect();

private:
	void							animateSprite(sf::Time dt);
	void							checkForNewState();

	void							faceDirection(sf::Vector2f mousePos);
	void							updateWeapons(sf::Time dt);
	void							movement(sf::Time dt);
//	void							lerpVelocity(bool lerpDown);
	virtual void					setupBody(b2World &world, float density, float friction) override;
	void							addGroundSensor();

	void							hurt(sf::Time dt);

private:
	FMOD::Channel					*mChannel;
	MeleeWeapon						mMeleeWeapon;
	RangeWeapon						mRangedWeapon;
	sf::FloatRect					mRect;
	sf::Vector2i					mSourcePos;
	Movement::Direction				mDirection;
	Movement::JumpState				mJumpState;

	b2Fixture*						mGroundSensor;

//	float							mTargetVelX;
	float							mAnimTimer;
	float							mLives;
	float							mInvincibilityTimer;

	bool							mMove;

	const float						SPEED = 12;
};

#endif // !PLAYER_H
