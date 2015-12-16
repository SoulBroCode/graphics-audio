#ifndef TORCH_H
#define TORCH_H
#include "SFML\Graphics.hpp"
#include <vector>
#include <memory>
#include "AudioManager.h"
//#include "RangeWeapon.h";

class Torch : public sf::Drawable {
public:
	Torch();
	void init(sf::Texture const &, sf::Vector2f const &, sf::IntRect const &);
	void update(float dt);
	void replaySound();
private:
	FMOD::Channel *mChannel;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	sf::Sprite mSprite;
	int mRowFrame;
	float mAnimTimer;

};
#endif