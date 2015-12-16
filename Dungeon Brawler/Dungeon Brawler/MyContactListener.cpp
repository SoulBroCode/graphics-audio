#include "MyContactListener.h"

void MyContactListener::BeginContact(b2Contact* contact) 
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	if (!groundCollision(bodyAUserData, bodyBUserData, contact, true))
	{
		if (!platformCollision(bodyAUserData, bodyBUserData, contact, true)){
			if (!weaponCollision(bodyAUserData, bodyBUserData, contact))
			{
				if (!dynamicObjectCollisions(bodyAUserData, bodyBUserData, contact, true))
					dynamicObjectCollisions(bodyBUserData, bodyAUserData, contact, true);
			}
		}
	}
}

void MyContactListener::EndContact(b2Contact* contact) 
{
	/*void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	groundCollision(bodyAUserData, bodyBUserData, contact, false);*/
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	platformCollision(bodyAUserData, bodyBUserData, contact, false);
}

bool MyContactListener::groundCollision(void*& a, void*& b, b2Contact*& contact, bool grounded)
{
	if (a == "Wall" || b == "Wall")
	{
		if (a == "Wall")
		{
			if (contact->GetFixtureB()->IsSensor())
			{
				sf::FloatRect ground = sf::FloatRect();
				if (grounded)
				{
					sf::Vector2f position = sf::Vector2f(contact->GetFixtureA()->GetBody()->GetPosition().x * 30, contact->GetFixtureA()->GetBody()->GetPosition().y * 30);

					for (b2Fixture* f = contact->GetFixtureA()->GetBody()->GetFixtureList(); f; f = f->GetNext())
					{
						b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();

						ground = sf::FloatRect(position, sf::Vector2f(-(polygonShape->GetVertex(0).x * 30) * 2, -(polygonShape->GetVertex(0).y * 30) * 2));
					}
				}
				static_cast<Entity*>(b)->grounded(grounded, ground);
			}
		}
		else
		{
			if (contact->GetFixtureA()->IsSensor())
			{
				sf::FloatRect ground = sf::FloatRect();
				if (grounded)
				{
					sf::Vector2f position = sf::Vector2f(contact->GetFixtureB()->GetBody()->GetPosition().x * 30, contact->GetFixtureB()->GetBody()->GetPosition().y * 30);

					for (b2Fixture* f = contact->GetFixtureB()->GetBody()->GetFixtureList(); f; f = f->GetNext())
					{
						b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();

						ground = sf::FloatRect(position, sf::Vector2f(-(polygonShape->GetVertex(0).x * 30) * 2, -(polygonShape->GetVertex(0).y * 30) * 2));
					}
				}
				static_cast<Entity*>(a)->grounded(grounded, ground);
			}
		}

		return true;
	}

	return false;
}
bool MyContactListener::platformCollision(void*& a, void*& b, b2Contact*& contact, bool startContact)
{
	if (a == "Platform" || b == "Platform")
	{
		if (a == "Platform")
		{
			if (contact->GetFixtureB()->IsSensor()){
				sf::FloatRect ground = sf::FloatRect();
				sf::Vector2f position = sf::Vector2f(contact->GetFixtureA()->GetBody()->GetPosition().x * 30, contact->GetFixtureA()->GetBody()->GetPosition().y * 30);
				for (b2Fixture* f = contact->GetFixtureA()->GetBody()->GetFixtureList(); f; f = f->GetNext())
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					ground = sf::FloatRect(position, sf::Vector2f(-(polygonShape->GetVertex(0).x * 30) * 2, -(polygonShape->GetVertex(0).y * 30) * 2));
				}
				if (startContact){
					static_cast<Entity*>(b)->landablePlatform(0);				
				}
				else{
					static_cast<Entity*>(b)->landablePlatform(-1);
				}
				static_cast<Entity*>(b)->grounded(true, ground);				
			}
		}
		else
		{
			if (contact->GetFixtureA()->IsSensor()){
				sf::FloatRect ground = sf::FloatRect();
				sf::Vector2f position = sf::Vector2f(contact->GetFixtureA()->GetBody()->GetPosition().x * 30, contact->GetFixtureA()->GetBody()->GetPosition().y * 30);
				for (b2Fixture* f = contact->GetFixtureA()->GetBody()->GetFixtureList(); f; f = f->GetNext())
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					ground = sf::FloatRect(position, sf::Vector2f(-(polygonShape->GetVertex(0).x * 30) * 2, -(polygonShape->GetVertex(0).y * 30) * 2));
				}
				if (startContact){
					static_cast<Entity*>(a)->landablePlatform(0);				
				}
				else{
					static_cast<Entity*>(a)->landablePlatform(-1);
				}
				static_cast<Entity*>(a)->grounded(true, ground);							
			}
		}

		return true;
	}
	return false;
}
bool MyContactListener::weaponCollision(void*& a, void*& b, b2Contact*& contact)
{
	if (contact->GetFixtureA()->GetBody()->GetType() == b2_staticBody || contact->GetFixtureB()->GetBody()->GetType() == b2_staticBody)
	{
		if (contact->GetFixtureA()->GetBody()->GetType() == b2_staticBody)
			static_cast<Entity*>(b)->sliced(static_cast<MeleeWeapon*>(a));
		else
			static_cast<Entity*>(a)->sliced(static_cast<MeleeWeapon*>(b));

		return true;
	}

	return false;
}

bool MyContactListener::dynamicObjectCollisions(void*& a, void*& b, b2Contact*& contact, bool inContact)
{
	Entity* eA = static_cast<Entity*>(a);
	Entity* eB = static_cast<Entity*>(b);

	bool wentInside = false;

	if (Player *p = dynamic_cast<Player *>(eA))
	{
		if (Projectile *p2 = dynamic_cast<Projectile *>(eB))
		{
			p->hit(p2->getDamage());
			p2->startContact();
		}
		else if (Enemy *p2 = dynamic_cast<Enemy *>(eB))
		{
			p->hit(0.5f);			//damage of the slime attack here
			p2->startContact();
		}
		wentInside = true;
	}
	else if (Projectile *p = dynamic_cast<Projectile *>(eA))
	{
		if (Player *p2 = dynamic_cast<Player *>(eB))
		{
			p->startContact();
			p2->hit(p->getDamage());
		}
		else if (Projectile *p2 = dynamic_cast<Projectile *>(eB))
		{
			p->startContact();
			p2->startContact();
		}
		else if (Enemy *p2 = dynamic_cast<Enemy *>(eB))
		{
			p->startContact();
			p2->hit(p->getDamage());
		}
		wentInside = true;
	}
	else if (Enemy *p = dynamic_cast<Enemy *>(eA))
	{
		if (Player *p2 = dynamic_cast<Player *>(eB))
		{
			p->startContact();
			p2->hit(0.5f);			//damage of the slime attack here
		}
		else if (Projectile *p2 = dynamic_cast<Projectile *>(eB))
		{
			p->hit(p2->getDamage());
			p2->startContact();
		}
		else if (Enemy *p2 = dynamic_cast<Enemy *>(eB))
		{
			p->startContact();
			p2->startContact();
		}
		wentInside = true;
	}

	return wentInside;
}