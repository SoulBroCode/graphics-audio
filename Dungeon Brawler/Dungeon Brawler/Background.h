#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "SFML\Graphics.hpp"
#include <vector>
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"





class Background : public sf::Drawable, private sf::NonCopyable {
public:
	Background();
	void create(TextureHolder &,const sf::Vector2f &);

private:
	//methods
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	//varibles
	std::vector<sf::Sprite> mSprite;

};
#endif