#ifndef SLIME_H
#define SLIME_H

#include "Enemy.h"

class Slime : public Enemy
{
public:
	enum class AnimState
	{
		Idle,
		Move,
		MeleeAttack,
		Jump,
		die,
	};

public:
					Slime();
	virtual void	init(TextureHolder& textureHolder, b2World &world, sf::Vector2f position) override;
	virtual void	update(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies) override;

protected:
	virtual void	idle(sf::Time dt) override;
	virtual void	patrol(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies) override;
	virtual void	follow(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies) override;
	virtual void	attack(sf::Time dt, sf::Vector2f playerPos, std::vector<std::unique_ptr<Enemy>>& enemies) override;
	virtual void	die(sf::Time dt) override;

	void			bounce();

private:
	MeleeWeapon		mMeleeWeapon;

	float			mInvincibilityTimer;
	const float		SPEED = 12;
};


#endif // !SLIME_H
