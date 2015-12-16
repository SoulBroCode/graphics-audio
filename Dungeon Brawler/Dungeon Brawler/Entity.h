#ifndef ENTITY_H
#define ENTITY_H

#include "ColliderIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "MovementIdentifiers.h"
#include "MeleeWeapon.h"
#include "Box2D\Box2D.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


class Entity : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
							Entity();
	void					setVelocity(sf::Vector2f velocity);

	virtual void			hit(float damage);
	virtual void			sliced(MeleeWeapon* weapon);
	virtual void			grounded(bool onGround, sf::FloatRect ground);
	virtual void			landablePlatform(int index);

	virtual void			startContact();
	virtual void			endContact();

	sf::Vector2f			getVelocity() const;
	sf::Vector2f			getOrigin() const;
	sf::Vector2f			getSize() const;
	bool					alive() const;
	bool					isHit() const;
	
protected:
	void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void			setupBody(b2World &world, float density, float friction);

protected:
	sf::Vector2f			mVelocity;
	sf::Vector2i			mSourcePos;
	sf::Vector2f			mSize;
	sf::Vector2f			mOrigin;
	sf::Sprite				mSprite;

	float					mScale;
	bool					mHit;
	bool					mAlive;
	bool					mContacting;

	b2Body*					mBody;
	sf::FloatRect			mGround;
};

#endif //ENTITY_H