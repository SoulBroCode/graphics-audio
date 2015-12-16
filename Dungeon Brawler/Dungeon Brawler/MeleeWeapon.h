#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"
#include "Box2D\Box2D.h"
#include "ColliderIdentifiers.h"
#include "OwnerOfWeapons.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(float damage = 0.5f, float range = 30, bool equiped = true, Origin origin = Origin::FromEnemy);
	void			setupBody(b2World &world, float density, float friction, sf::Vector2f positon = sf::Vector2f(0,0));
	
	void			update(sf::Vector2f position, bool left, sf::Time dt);
	void			attack(sf::Vector2f position, bool left);
	void			notAttacking();

	virtual void	levelUp() override;

	sf::Vector2f	getPosition();

private:
	void			timer(sf::Time dt);

private:
	b2Body*			mBody;
	Origin			mOrigin;
	float			mAttackTimer;

	const float		MAX_TIMER = 0.1f;
};

#endif // !MELEEWEAPON_H