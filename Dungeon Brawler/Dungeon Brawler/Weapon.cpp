#include "Weapon.h"

Weapon::Weapon(float damage, float range, bool equiped)
	: mMasteryLevel(0),
	mDamage(damage),
	mRange(range),
	mEquiped(equiped),
	mAttack(false)
{
}

const bool Weapon::getEquiped() const
{
	return mEquiped;
}

const float Weapon::getDamage() const
{
	return mDamage;
}

void Weapon::setEquiped(bool equiped)
{
	mEquiped = equiped;
}