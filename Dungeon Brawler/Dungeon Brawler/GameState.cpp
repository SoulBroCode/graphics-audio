#include "GameState.h"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window)
{
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	mWorld.processEvents(event);

	// Escape pressed, trigger the pause screen
	if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7))
		requestStackPush(States::ID::Pause);

	return true;
}