#include "Hud.h"

void Hud::init(float lifes, TextureHolder& textures)
{
	mCurrentHealth = lifes;

	int lifesNeeded = lifes / 2;
	if (lifes - (int)lifes == 0.5f)
		lifesNeeded++;

	for (int i = 0; i < lifesNeeded; i++)
	{
		float lifeValue = 0;
		if (lifes - 1 >= 0)
			lifeValue = 1;
		else
			lifeValue = 0.5f;

		Life life = Life();
		life.init(lifeValue, textures, sf::Vector2f(10, 10) + sf::Vector2f(90 * i, 0));
		mHearts.push_back(life);

		lifes -= lifeValue;
	}
}

void Hud::update(float lives)
{
	//UPDATE LIVES
	if (mCurrentHealth - lives != 0)
	{
		float damage = mCurrentHealth - lives;
		for (int i = mHearts.size() - 1; i >= 0; i--)
		{
			if (mHearts.at(i).getLives() != 0)
			{
				damage = mHearts.at(i).reduceLives(damage);
				if (damage == 0)
					break;
			}
		}
	}
}

void Hud::addLives(float life, TextureHolder& textures)
{
	
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//states.transform *= getTransform();		//USED WHEN INHERITING FROM TRANSFORMABLE

	for (int i = 0; i < mHearts.size(); i++)
		target.draw(mHearts.at(i).getSprite(), states);			//USED TO DRAW OBJECT
}