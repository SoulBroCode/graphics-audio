#include "Lifes.h"

void Life::init(float lifes, TextureHolder& textures, sf::Vector2f position)
{
	mLifes = lifes;

	mSprite.setTexture(textures.get(Textures::ID::Heart));
	mSprite.setPosition(position);

	checkLives();
}

float Life::reduceLives(float damage)
{
	if (damage - mLifes >= 0)
	{
		mLifes = 0;
		damage -= mLifes;
	}
	else
	{
		mLifes -= damage;
		damage = 0;
	}

	checkLives();

	return damage;
}

void Life::checkLives()
{
	if (mLifes == 1)
		mFrame = 0;
	else if (mLifes == 0.5f)
		mFrame = 1;
	else
		mFrame = 2;

	mSprite.setTextureRect(sf::IntRect(84 * mFrame, 0, 84, 64));
}

const sf::Sprite& Life::getSprite() const
{
	return mSprite;
}
float Life::getLives() const
{
	return mLifes;
}