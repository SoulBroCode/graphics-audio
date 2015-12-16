#ifndef APPLICATION_H
#define APPLICATION_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "StateStack.h"

#include "State.h"
#include "StateIdentifiers.h"
#include "Utility.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
public:
							Application();
	void					run();


private:
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	void					updateStatistics(sf::Time dt);
	void					registerStates();


private:
	const sf::Time	TIME_PER_FRAME = sf::seconds(1.f / 60.f);

	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;

	StateStack				mStateStack;

	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
};

#endif // !APPLICATION_H
