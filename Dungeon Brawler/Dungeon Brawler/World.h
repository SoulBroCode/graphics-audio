#ifndef WORLD_H
#define WORLD_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "Slime.h"
#include "MyContactListener.h"
#include "FloorGenerator.h"

#include "Background.h"
#include "Box2D\Box2D.h"
//#include "Hud.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Joystick.hpp>

#include <algorithm>
#include <cmath>
#include <array>
#include <queue>
#include <time.h>

#include "AudioManager.h";
#include "Torch.h";
// Forward declaration
namespace sf
{
	class RenderWindow;
}


struct Camera
{
public:
	void							init(sf::Vector2f position);
	void							update(sf::Vector2f mousePos, sf::Time dt, const Player* player, sf::View& worldView);

	sf::Vector2f					getPosition() const;

private:
	const float						CAMERA_X_VEL = 500.f;
	sf::Vector2f					mPosition;
	sf::Vector2f					mTargetPosition;
};



class World : private sf::NonCopyable
{
public:
	explicit						World(sf::RenderWindow& window);
	void							update(sf::Time dt);
	void							draw();

	void							processEvents(sf::Event event);

	void							handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void							handleRealTimeInput();
	void							handleMouseInput();

	void							initializeController();
	void							handleXboxJoystickInput();
	void							handleXboxButtonInput();

private:
	void							loadTextures();


private:
	sf::Texture						mGroundTexture;
	sf::Texture						mBoxTexture;

	b2Vec2							mGravity;

	const float						SCALE = 30.f;
	bool							mControllerConnected;
	bool							mUseStickForMovement;

	sf::RenderWindow&				mWindow;
	sf::View						mWorldView;
	sf::FloatRect					mWorldBounds;

	std::unique_ptr<Camera>			mCamera;
	//std::unique_ptr<Hud>			mHud;

	sf::Vector2f					mCursorPos;
	std::vector<bool>				mButtonsPressed;

	TextureHolder					mTextures;
	std::unique_ptr<Player>			mPlayer;

	MyContactListener				mContactListener;
	b2World							mWorld;

	std::shared_ptr<AudioManager>	mAudioManager;
	std::unique_ptr<FloorGenerator> mFloorGenerator;
	std::unique_ptr<Background>		mBackground;

	
	std::vector<std::unique_ptr<Projectile>>	mProjectiles;
};

#endif // !WORLD_H
