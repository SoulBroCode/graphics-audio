#ifndef HUD_H
#define HUD_H

#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "Lifes.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>

class Hud : public sf::Drawable, private sf::NonCopyable
{
public:
	void					init(float lives, TextureHolder& textures);
	void					update(float lives);

	void					addLives(float lives, TextureHolder& textures);
	
private:
	void					draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<Life>		mHearts;
	float					mCurrentHealth;
};


#endif //HUD_H