#ifndef PLATFORM_H
#define PLATFORM_H
#include "SFML\Graphics.hpp"
#include <vector>
#include <memory>
#include <AudioManager.h>
#include "Box2D\Box2D.h"
class Platform : public sf::Drawable {
public:
	Platform();
	enum class Type
	{
		Null, Middle, Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight, HalfMiddle, HalfLeft, HalfRight
	};//clockwise
	enum class EntityTrap
	{
		Null, ArrowShooter = 13, Torch = 18, Enemy = 19
	};//clockwise
	void init(sf::Texture const &, sf::Vector2f const &, sf::IntRect const &);
	void setOrigin(sf::Vector2f const &);

	void setType(Type const &);

	sf::Vector2f const & getPos();
	Type const &  getType();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	sf::Sprite mSprite;
	Type mType;


};
#endif