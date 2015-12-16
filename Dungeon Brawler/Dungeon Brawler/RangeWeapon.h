#ifndef RANGEWEAPON_H
#define RANGEWEAPON_H

#include "Weapon.h"
#include "Projectile.h"
#include "OwnerOfWeapons.h"

#include <memory.h>

class RangeWeapon : public Weapon
{
public:
								RangeWeapon(float damage = 0.5f, float range = 150, bool equiped = true, ProjectileType type = ProjectileType::Arrow, Origin = Origin::FromEnemy);
	virtual void				levelUp() override;

	void						update(sf::Time dt);

	std::unique_ptr<Projectile> createProjectile();
	const float					getRange() const;
	const bool					canFire() const;

private:
	void						reloadTimer(sf::Time dt);

private:
	bool						mCanFire;
	float						mFireRate;
	ProjectileType				mAmmoType;
	Origin						mOwner;
};

#endif // !RANGEWEAPON_H