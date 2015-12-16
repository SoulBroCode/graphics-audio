#include "MenuState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mOptions()
	, mOptionIndex(0)
{
	sf::Texture& texture = context.textures->get(Textures::ID::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::ID::Main);

	mBackgroundSprite.setTexture(texture);
	mBackgroundSprite.setScale(context.window->getSize().x / (float)mBackgroundSprite.getTextureRect().width,
							   context.window->getSize().y / (float)mBackgroundSprite.getTextureRect().height);

	// A simple menu demonstration
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	playOption.setCharacterSize(context.window->getSize().y / 20);
	mOptions.push_back(playOption);

	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, context.window->getSize().y / 20));
	exitOption.setCharacterSize(context.window->getSize().y / 20);
	mOptions.push_back(exitOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	//window.draw(mBackgroundSprite);

	for (const sf::Text& text : mOptions)
		window.draw(text);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed && !sf::Joystick::isConnected(0))
		return false;

	if (event.key.code == sf::Keyboard::Return || sf::Joystick::isButtonPressed(0, 0))
	{//ENTER || A is pressed then enter
		if (mOptionIndex == Play)
		{
			requestStackPop();
			requestStackPush(States::ID::Game);
		}
		else if (mOptionIndex == Exit)
		{
			// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement and wrap-around
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mOptions.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment and wrap-around
		if (mOptionIndex < mOptions.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		updateOptionText();
	}

	else if (event.type == sf::Event::JoystickMoved)
	{
		float movementY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float movementY2 = sf::Joystick::getAxisPosition(0, sf::Joystick::PovY);
		if (movementY > 50.f || movementY < -50.f || movementY2 > 50.f || movementY2 < -50.f)
		{
			if (movementY > 50.f || movementY2 < -50.f)
			{
				// Increment and wrap-around
				if (mOptionIndex < mOptions.size() - 1)
					mOptionIndex++;
				else
					mOptionIndex = 0;
			}
			else if (movementY < -50.f || movementY2 > 50.f)
			{
				// Decrement and wrap-around
				if (mOptionIndex > 0)
					mOptionIndex--;
				else
					mOptionIndex = mOptions.size() - 1;
			}
			updateOptionText();
		}
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// White all texts
	for (sf::Text& text : mOptions)
		text.setColor(sf::Color::White);

	// Red the selected text
	mOptions[mOptionIndex].setColor(sf::Color::Red);
}