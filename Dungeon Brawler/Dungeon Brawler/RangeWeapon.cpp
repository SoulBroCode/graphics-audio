#include "RangeWeapon.h"

RangeWeapon::RangeWeapon(float damage, float range, bool equiped, ProjectileType type, Origin origin)
	:Weapon(damage, range, equiped), mAmmoType(type), mCanFire(true), mFireRate(0), mOwner(origin)
{}

void RangeWeapon::levelUp()
{
	mMasteryLevel++;
	mRange += 50;
	mDamage += 0.125;
}

void RangeWeapon::update(sf::Time dt)
{
	reloadTimer(dt);
}

std::unique_ptr<Projectile> RangeWeapon::createProjectile()
{
	mCanFire = false;
	std::unique_ptr<Projectile> ptr(new Projectile(mDamage, mRange, mAmmoType, mOwner));
	return ptr;
}

void RangeWeapon::reloadTimer(sf::Time dt)
{
	const float FIRE_RATE = 0.5f;

	if (!mCanFire)
	{
		mFireRate += dt.asSeconds();

		if (mFireRate >= FIRE_RATE)
		{
			mCanFire = true;
			mFireRate = 0;
		}
	}
}

const bool RangeWeapon::canFire() const
{
	return mCanFire;
}

const float RangeWeapon::getRange() const
{
	return mRange;
}