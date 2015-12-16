#include "World.h"

void Camera::init(sf::Vector2f position)
{
	mTargetPosition = mPosition = position;
}
void Camera::update(sf::Vector2f mousePos, sf::Time dt, const Player* player, sf::View& worldView)
{
	if (player->attackRanged())
	{
		sf::Vector2f offset = sf::Vector2f(worldView.getSize().x / 6, 0);
		if (player->getDirection() == Movement::Direction::Right)
		{
			mTargetPosition = player->getPosition() + offset;
			mousePos += offset;
		}
		else
		{
			mTargetPosition = player->getPosition() - offset;
			mousePos -= offset;
		}
	}
	else
	{
		mTargetPosition = player->getPosition();
	}

	if (mPosition != mTargetPosition)
	{
		if (mPosition.x < mTargetPosition.x)
		{
			mPosition.x += (CAMERA_X_VEL + player->getVelocity().x) * dt.asSeconds();
			if (mPosition.x > mTargetPosition.x)
				mPosition.x = mTargetPosition.x;
		}
		else
		{
			mPosition.x -= (CAMERA_X_VEL + player->getVelocity().x) * dt.asSeconds();
			if (mPosition.x < mTargetPosition.x)
				mPosition.x = mTargetPosition.x;
		}
	}

	mPosition.y = mTargetPosition.y;
	worldView.setCenter(mPosition);
}

sf::Vector2f Camera::getPosition() const
{
	return mPosition;
}




World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x * 2, mWorldView.getSize().y * 2)
	, mTextures()
	, mPlayer(new Player())
	, mCamera(new Camera())
	//, mHud(new Hud)
	, mFloorGenerator(new FloorGenerator())
	, mBackground(new Background())
	, mGravity(0.f, 9.8f)
	, mWorld(mGravity)
	, mUseStickForMovement(true)
{
	loadTextures();
	//Load Audio
	mAudioManager = AudioManager::sharedAudioManager();
	FMOD::Channel *channel = NULL;
	mAudioManager->playMusic(mAudioManager->mBGSound, &channel);
	channel->setVolume(0.05f);

	//Player
	mPlayer->init(mTextures, mWorld, sf::Vector2f(300, 1600));
	//Create the level
	mFloorGenerator->loadFile("Media/Data/gamemap.json");
	mFloorGenerator->makeRooms(mTextures, mWorld);
	mBackground->create(mTextures, sf::Vector2f(34, 22));



	// Prepare the view
	mCamera->init(mPlayer->getPosition());
	mWorldView.setCenter(mCamera->getPosition());
	mWorldView.zoom(0.75f);

	mWorld.SetContactListener(&mContactListener);

	/** Prepare the world */
	mGroundTexture.loadFromFile("Media/Textures/square.jpg");
	mBoxTexture.loadFromFile("Media/Textures/box.png");

	//mHud->init(mPlayer->getLives(), mTextures);
	initializeController();
}

void World::loadTextures()
{
	mTextures.load(Textures::ID::Player, "Media/Textures/Player.png");
	mTextures.load(Textures::ID::Slime, "Media/Textures/slime.png");
	mTextures.load(Textures::ID::Arrow, "Media/Textures/Arrow.png");
	mTextures.load(Textures::ID::Tilemap, "Media/Textures/Tilemap.png");
	mTextures.load(Textures::ID::Square, "Media/Textures/square.jpg");
	mTextures.load(Textures::ID::Torch, "Media/Textures/torch.png");
	//mTextures.load(Textures::ID::Heart, "Media/Textures/BlackHearts.png");
}

void World::initializeController()
{
	//get information about the joystick
	sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
	std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;


	//query joystick for settings if it's plugged in...
	if (mControllerConnected = sf::Joystick::isConnected(0))
	{
		// check how many buttons joystick number 0 has
		unsigned int buttonCount = sf::Joystick::getButtonCount(0);
		for (int i = 0; i < buttonCount; i++)
			mButtonsPressed.push_back(false);

		// check if joystick number 0 has a Z axis
		bool hasZ = sf::Joystick::hasAxis(0, sf::Joystick::Z);

		std::cout << "Button count: " << buttonCount << std::endl;
		std::cout << "Has a z-axis: " << hasZ << std::endl;
	}
}

void World::processEvents(sf::Event event)
{
	mWindow.setKeyRepeatEnabled(true);
	if (mControllerConnected)
		handleXboxButtonInput();
	else
	{
		handleRealTimeInput();
		handleMouseInput();
	}

	switch (event.type)
	{
	case sf::Event::KeyPressed:
		handlePlayerInput(event.key.code, true);
		break;

	case sf::Event::KeyReleased:
		handlePlayerInput(event.key.code, false);
		break;

	case sf::Event::JoystickMoved:
		handleXboxJoystickInput();
		break;

	case sf::Event::Closed:
		mWindow.close();
		break;
	}

	if (event.type == sf::Event::KeyPressed)
	{
		mWindow.setKeyRepeatEnabled(false);
		if (event.key.code == sf::Keyboard::Num1){
			mAudioManager->changeMusicState();
		}
		else if (event.key.code == sf::Keyboard::Num2){
			mAudioManager->changeEffectState();
		}
		else if (event.key.code == sf::Keyboard::Num3){
			mAudioManager->change3DAudioState();
			const std::vector<std::shared_ptr<Torch>> s = mFloorGenerator->GetCurrentRoom()->mTorches;
			for (const std::shared_ptr<Torch> & p : s)
			{
				p->replaySound();
			}
		}
		else if (event.key.code == sf::Keyboard::Num4){
			mAudioManager->changeReverbState();
		}
	}
}

void World::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	////////////////////////////////WEAPON SWITCH////////////////////////////////////////
	if (key == sf::Keyboard::Q)
	{
		if (isPressed)
			mPlayer->switchWeapon();
	}
	/////////////////////////////////////////////////////////////////////////////////////
}

void World::handleRealTimeInput()
{
	//////////////////////////////////////JUMP/////////////////////////////////////////////
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		mPlayer->setAnimState(Player::AnimState::Jump, mPlayer->getDirection());
	}
	///////////////////////////////////MOVEMENT///////////////////////////////////////////
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		mPlayer->setAnimState(Player::AnimState::Move, Movement::Direction::Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		mPlayer->setAnimState(Player::AnimState::Move, Movement::Direction::Right);
	}
	else
		mPlayer->setAnimState(Player::AnimState::Idle, mPlayer->getDirection());
	//////////////////////////////////ITEM USE///////////////////////////////////////////
}

void World::handleMouseInput()
{
	///////////////////////////////////ATTACKS///////////////////////////////////////////
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (mPlayer->attackRanged())
		{
			if (mPlayer->canFire())
			{
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(mWindow);
				sf::Vector2f screenRelativeToPlayer = mPlayer->getPosition() - (sf::Vector2f)(mWindow.getSize() / 2u);

				mProjectiles.push_back(std::move(mPlayer->createProjectile(screenRelativeToPlayer + mousePos, sf::Vector2f(mWindow.getSize().x / 6.f, 0), 1.0f)));
				mProjectiles.at(mProjectiles.size() - 1)->setUpProjectile(mTextures, mWorld);
			}
		}
		else
		{
			mPlayer->swingSword();
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
}

void World::handleXboxJoystickInput()
{
	sf::Vector2f movementDirection;
	if (mUseStickForMovement)
	{
		//LEFT STICK
		movementDirection = sf::Vector2f(
							sf::Joystick::getAxisPosition(0, sf::Joystick::X),
							sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
	}
	else
	{
		//ARROW PAD
		movementDirection = sf::Vector2f(
			sf::Joystick::getAxisPosition(0, sf::Joystick::PovX),
			sf::Joystick::getAxisPosition(0, sf::Joystick::PovY));
	}

	//RIGHT STICK
	sf::Vector2f cursorDirection = sf::Vector2f(
		sf::Joystick::getAxisPosition(0, sf::Joystick::U),
		sf::Joystick::getAxisPosition(0, sf::Joystick::R));


	if (movementDirection.x > 25.f || movementDirection.x < -25.f)// || movementDirection.y > 50.f || movementDirection.y < -50.f)
	{
		if (movementDirection.x > 25.f)
		{//RIGHT
			mPlayer->setAnimState(Player::AnimState::Move, Movement::Direction::Right);
		}
		else if (movementDirection.x < -25.f)
		{//LEFT
			mPlayer->setAnimState(Player::AnimState::Move, Movement::Direction::Left);
		}
		/*else
		{
			mPlayer->setAnimState(Player::AnimState::Idle, mPlayer->getDirection());
		}

		if (movementDirection.y > 50.f)
		{//DOWN for left stick
			if (!mUseStickForMovement)
				mPlayer->setAnimState(Player::AnimState::Jump, mPlayer->getDirection());
		}
		else if (movementDirection.y < -50.f)
		{//UP for left stick
			if (mUseStickForMovement)
				mPlayer->setAnimState(Player::AnimState::Jump, mPlayer->getDirection());
		}*/
	}
	else
	{
		mPlayer->setAnimState(Player::AnimState::Idle, mPlayer->getDirection());
	}


	if (cursorDirection.x > 50.f || cursorDirection.x < -50.f || cursorDirection.y > 50.f || cursorDirection.y < -50.f)
	{
		mCursorPos = cursorDirection;
	}
}

void World::handleXboxButtonInput()
{
	if (sf::Joystick::isButtonPressed(0, 0))
	{//"A" button on the XBox 360 controller
		if (mButtonsPressed.at(0) == false)
		{
			std::cout << "A" << std::endl;;;;;;;
			mPlayer->setAnimState(Player::AnimState::Jump, mPlayer->getDirection());
			mButtonsPressed.at(0) = true;;
		}
	}
	else
		mButtonsPressed.at(0) = false;;;

	if (sf::Joystick::isButtonPressed(0, 1))
	{//"B" button on the XBox 360 controller
		if (mButtonsPressed.at(1) == false)
		{
			std::cout << "B" << std::endl;;
			//B TO DROP
			mButtonsPressed.at(1) = true;
		}
	}
	else
		mButtonsPressed.at(1) = false;

	if (sf::Joystick::isButtonPressed(0, 2))
	{//"X" button on the XBox 360 controller
		if (mButtonsPressed.at(2) == false)
		{
			std::cout << "X" << std::endl;

			mButtonsPressed.at(2) = true;
		}
	}
	else
		mButtonsPressed.at(2) = false;

	if (sf::Joystick::isButtonPressed(0, 3))
	{//"Y" button on the XBox 360 controller
		if (mButtonsPressed.at(3) == false)
		{
			std::cout << "Y" << std::endl;
			mPlayer->switchWeapon();

			mButtonsPressed.at(3) = true;
		}
	}
	else
		mButtonsPressed.at(3) = false;

	if (sf::Joystick::isButtonPressed(0, 4))
	{//"LB" button on the XBox 360 controller
		if (mButtonsPressed.at(4) == false)
		{
			std::cout << "LB" << std::endl;

			mButtonsPressed.at(4) = true;
		}
	}
	else
		mButtonsPressed.at(4) = false;

	if (sf::Joystick::isButtonPressed(0, 5))
	{//"RB" button on the XBox 360 controller
		if (mButtonsPressed.at(5) == false)
		{
			std::cout << "RB" << std::endl;
			if (mPlayer->attackRanged())
			{
				if (mPlayer->canFire())
				{
					mProjectiles.push_back(std::move(mPlayer->createProjectile(mCursorPos + mPlayer->getPosition(), sf::Vector2f(0, 0), 1.0f)));
					mProjectiles.at(mProjectiles.size() - 1)->setUpProjectile(mTextures, mWorld);
				}
			}
			else
			{
				mPlayer->swingSword();
			}

			mButtonsPressed.at(5) = true;
		}
	}
	else
		mButtonsPressed.at(5) = false;

	if (sf::Joystick::isButtonPressed(0, 6))
	{//"select" button on the XBox 360 controller
		if (mButtonsPressed.at(6) == false)
		{
			std::cout << "select" << std::endl;

			mButtonsPressed.at(6) = true;
		}
	}
	else
		mButtonsPressed.at(6) = false;

	if (sf::Joystick::isButtonPressed(0, 7))
	{//"start" button on the XBox 360 controller
		if (mButtonsPressed.at(7) == false)
		{
			std::cout << "start" << std::endl;

			mButtonsPressed.at(7) = true;
		}
	}
	else
		mButtonsPressed.at(7) = false;

	if (sf::Joystick::isButtonPressed(0, 8))
	{//"Left Trigger" button on the XBox 360 controller
		if (mButtonsPressed.at(8) == false)
		{
			std::cout << "Left Trigger" << std::endl;

			mButtonsPressed.at(8) = true;
		}
	}
	else
		mButtonsPressed.at(8) = false;

	if (sf::Joystick::isButtonPressed(0, 9))
	{//"Right Trigger" button on the XBox 360 controller
		if (mButtonsPressed.at(9) == false)
		{
			std::cout << "Right Trigger" << std::endl;

			mButtonsPressed.at(9) = true;
		}
	}
	else
		mButtonsPressed.at(9) = false;
}

void World::update(sf::Time dt)
{
	mWorld.Step(1 / 60.f, 8, 3);

	sf::Vector2f mousePos;
	sf::Vector2f screenRelativeToPlayer;
	if (!mControllerConnected)
	{
		mousePos = (sf::Vector2f)sf::Mouse::getPosition(mWindow);
		screenRelativeToPlayer = mPlayer->getPosition() - (sf::Vector2f)(mWindow.getSize() / 2u);
	}
	else
	{
		mousePos = mCursorPos;
		screenRelativeToPlayer = mPlayer->getPosition();
	}

	mCamera->update(mousePos, dt, mPlayer.get(), mWorldView);
	mPlayer->update(dt, screenRelativeToPlayer + mousePos);
	mFloorGenerator->GetCurrentRoom()->update(dt.asMilliseconds());

	const std::vector<std::shared_ptr<ArrowShooter>> s = mFloorGenerator->GetCurrentRoom()->mArrowShooters;
	sf::FloatRect playerRect = mPlayer->getRect();
	for (const std::shared_ptr<ArrowShooter> & p : s)
	{
		if (p->getCanFire()){
			if (p->getLineOfSight().intersects(playerRect)){
				mProjectiles.push_back(std::move(p->createProjectile(0.8f)));
				mProjectiles.at(mProjectiles.size() - 1)->setUpProjectile(mTextures, mWorld);
				p->setCanFire(false);
				if (p->getPauseSound()){
					p->setPauseSound(false);
				}
			}
			else{
				if (!p->getPauseSound()){
					p->setPauseSound(true);
				}
			}
		}
		else{
			p->update(dt.asMilliseconds());
		}
	}
	//mHud->update(mPlayer->getLives());



	for (std::unique_ptr<Projectile>& p : mProjectiles)
	{
		p->update(dt);
	}
	
	for (std::unique_ptr<Enemy>& s : mFloorGenerator->GetCurrentRoom()->mEnemies)
	{
		s->update(dt, mPlayer->getPosition(), mFloorGenerator->GetCurrentRoom()->mEnemies);
	}
}

void World::draw()
{
	mAudioManager->update(); 

	mWindow.setView(mWorldView);
	mWindow.draw(*mBackground);
	mWindow.draw(*mFloorGenerator);

	mWindow.draw(*mPlayer);


	for (std::unique_ptr<Projectile>& p : mProjectiles)
	{
		mWindow.draw(*p);
	}

	sf::CircleShape circle(400, 40);
	circle.setPosition(sf::Vector2f(2500, 350));
	circle.setOrigin(400, 400);
	circle.setFillColor(sf::Color(0, 0, 0, 0));
	circle.setOutlineColor(sf::Color(255, 255, 255, 180));
	circle.setOutlineThickness(1.0f);
	mWindow.draw(circle);
	//mWindow.draw(*mHud);

	///////////////////use this code for testing purpose///////////////////////////////////////////////////////////////
	for (b2Body* BodyIterator = mWorld.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_circle)
				{
				}
				else if (shapeType == b2Shape::e_polygon)
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();
					
					sf::ConvexShape sprite;
					int lenght = polygonShape->GetVertexCount();
					sprite.setPointCount(lenght);
					for (int i = 0; i < lenght; i++){
						sprite.setPoint(i, sf::Vector2f(polygonShape->GetVertex(i).x, polygonShape->GetVertex(i).y));
					}
					sprite.setFillColor(sf::Color(0, 0, 0, 0));
					sprite.setOutlineColor(sf::Color(255, 255, 255, 180));
					sprite.setOutlineThickness(-0.025f);
					sprite.setScale(30.f, 30.f);
					sprite.setOrigin(0, 0);
					sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
					sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
					mWindow.draw(sprite);
				}
			}
		}
	}
}