#ifndef LIFES_H
#define LIFES_H

#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>

class Life
{
public:
	void				init(float lifes, TextureHolder& textures, sf::Vector2f position);

	float				reduceLives(float damage);
	const sf::Sprite&	getSprite() const;
	float				getLives() const;

private:
	void				checkLives();

private:
	sf::Sprite			mSprite;

	float				mLifes;
	int					mFrame;
};

#endif //LIFES_H