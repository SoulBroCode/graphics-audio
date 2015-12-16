#include "Entity.h"

Entity::Entity() : mBody(nullptr), mGround(sf::FloatRect(0,0, 0,0))
{}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();		//USED WHEN INHERITING FROM TRANSFORMABLE
	target.draw(mSprite, states);			//USED TO DRAW OBJECT
}

void Entity::hit(float damage) {}
void Entity::sliced(MeleeWeapon* weapon) {}
void Entity::grounded(bool onGround, sf::FloatRect ground) {}

void Entity::landablePlatform(int index){}

void Entity::setupBody(b2World &world, float density, float friction)
{
	const float scale = 30.f;

	//creating body
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2((getPosition().x) / scale, (getPosition().y) / scale);
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	mBody = world.CreateBody(&bodyDef);
	mBody->SetUserData(this);

	b2PolygonShape shape;
	shape.SetAsBox((mSize.x / 2) / scale, (mSize.y / 2) / scale);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	fixtureDef.shape = &shape;

	mBody->CreateFixture(&fixtureDef);
}


void Entity::startContact()
{ 
	mContacting = true; 
}
void Entity::endContact()
{
	mContacting = false;
}

sf::Vector2f Entity::getOrigin() const
{
	return mOrigin;
}

sf::Vector2f Entity::getSize() const
{
	return mSize;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

bool Entity::isHit() const
{
	return mHit;
}

bool Entity::alive() const
{
	return mAlive;
}
