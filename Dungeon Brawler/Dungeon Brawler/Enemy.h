#ifndef MELEEENEMY_H
#define MELEEENEMY_H

#include "Entity.h"
#include "MeleeWeapon.h"
#include "AudioManager.h"

class Enemy : public Entity
{
public:
	enum class State
	{
		Idle,
		Patrol,
		Follow,
		Attack,
		Die,
	};

	enum class AnimState
	{
		Idle,
		Move,
		Attack,
		Jump,
		die,
	};

	enum class Type
	{
		Slime,
		Bat,
		Goblin,
		Ogre,
		Golem,
	};

public:
							Enemy();
	virtual void			init(TextureHolder& textureHolder, b2World &world, sf::Vector2f position);
	virtual void			update(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies);

	virtual void			hit(float damage) override final;
	virtual void			sliced(MeleeWeapon* weapon) override final;
	virtual void			grounded(bool onGround, sf::FloatRect ground) override final;

	virtual void			startContact() override final;
	virtual void			endContact() override final;


	float					getLives() const;

protected:
	void					animateSprite(sf::Time dt);

	virtual void			idle(sf::Time dt);
	virtual void			attack(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies);
	virtual void			follow(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies);
	virtual void			patrol(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies);
	virtual void			die(sf::Time dt);

	void					hurt(sf::Time dt);

	virtual void			setupBody(b2World &world, float density, float friction) final;
	void					addGroundSensor();

	void					canSeePlayer(sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies);
	void					checkForNewState();
protected:
	FMOD::Channel			*mChannel;
	State					mState;
	Type					mType;

	sf::Vector2i			mSourcePos;
	Movement::Direction		mDirection;
	Movement::JumpState		mJumpState;

	float					mAnimTimer;
	float					mInvincibilityTimer;
	float					mLives;

	bool					mMove;

	const float				SPEED = 25;
	const float				JUMP_SPEED = 40;
};

#endif //MELEEENEMY_H