#ifndef WEAPON_H
#define WEAPON_H

#include "SFML\Graphics.hpp"

class Weapon
{
public:
					Weapon(float damage = 0, float range = 0, bool equiped = true);
	virtual void	levelUp() = 0;

	const bool		getEquiped() const;
	const float		getDamage() const;
	void			setEquiped(bool equiped);

protected:
	unsigned int	mMasteryLevel;
	float			mDamage;
	float			mRange;
	bool			mEquiped;
	bool			mAttack;
};

#endif // !WEAPON_H
