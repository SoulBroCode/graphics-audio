#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include "OwnerOfWeapons.h"
#include "AudioManager.h"
class Projectile : public Entity
{
public:
						Projectile(float maxDamage, float range, ProjectileType type, Origin origin);
	void				setUpArrow(sf::Vector2f dir, float pwrForce, sf::Vector2f position);
	void				setUpProjectile(TextureHolder& textures, b2World &world);
	void				newPosition(sf::Vector2f position);

	void				update(sf::Time dt);

	virtual void		startContact() override;
	virtual void		endContact() override;

	virtual void		hit(float damage) override;
	virtual void		grounded(bool onGround, sf::FloatRect ground) override;

	float				getDamage() const;
	const sf::Sprite&	getSprite() const;

private:
	virtual void		setupBody(b2World &world, float density, float friction) override;
	void				addGroundSensor();
	void				calculateVelocity(float pwrForce, sf::Vector2f dir);
	void				calculateRotation();

private:
	ProjectileType		mType;
	Origin				mOrigin;

	float				mDamage;
	float				mSpeed;

	bool				mOnFire;
};

#endif //PROJECTILE_H