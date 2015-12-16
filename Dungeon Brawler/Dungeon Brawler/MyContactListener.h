#ifndef MYCONTACTLISTENER_H
#define MYCONTACTLISTENER_H

#include "Box2D\Box2D.h"

#include "Player.h"
#include "Projectile.h"
#include "MeleeWeapon.h"
#include "Enemy.h"


class MyContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	bool groundCollision(void*& a, void*& b, b2Contact*& contact, bool grounded);
	bool platformCollision(void*& a, void*& b, b2Contact*& contact, bool startContact);
	bool weaponCollision(void*& a, void*& b, b2Contact*& contact);
	bool dynamicObjectCollisions(void*& a, void*& b, b2Contact*& contact, bool inContact);
};

#endif // !MYCONTACTLISTENER_H