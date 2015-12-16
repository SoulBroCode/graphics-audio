#include "Room.h"

Room::Room(){

}
//startingpos, bool start
bool Room::makePlatformTile(const TextureHolder &textureHolder, const int &num, const int &row, const int &col){
	int height = 50;
	int width = height;
	bool isCorner = false;

	if (num != (int)Platform::Type::Middle && num != (int)Platform::Type::HalfMiddle)
	{
		isCorner = true;
	}
	std::unique_ptr<Platform> platform(new Platform());
	if (num < (int)Platform::Type::HalfMiddle){
		platform->init(textureHolder.get(Textures::ID::Square),
			sf::Vector2f(row * width, col * height),
			sf::IntRect(0, 0, 50, 50));
	}
	else{
		sf::Vector2f position = sf::Vector2f(row * width, col * height - 20);
		platform->init(textureHolder.get(Textures::ID::Square),
			position,
			sf::IntRect(0, 0, height, width*0.20f));
	}
	
	platform->setType(static_cast<Platform::Type>(num));
	mPlatforms.push_back(std::move(platform));


	return isCorner;
}

void Room::makePlatformColliders(std::vector<int> &cornerIndex, b2World &world){
	const float scale = 0.033333333f; //use 0.033333f for mutiplecation instead of 30 for division.
	float blockWidth = 50;

	std::vector<int>::iterator end = --cornerIndex.end(); //end at second last 
	for (std::vector<int>::iterator i = cornerIndex.begin(); i < end; i++) {
		std::shared_ptr<Platform> platform = mPlatforms[*i];
		if (platform->getType() == Platform::Type::TopLeft){
			makeHorizontalColliders(world, platform, i, end);
			makeVerticalColliders(world, platform, i, end);
		}
		else if (platform->getType() == Platform::Type::Left || platform->getType() == Platform::Type::BottomLeft){
			makeHorizontalColliders(world, platform, i, end);
		}
		else if (platform->getType() == Platform::Type::Top || platform->getType() == Platform::Type::TopRight){
			makeVerticalColliders(world, platform, i, end);
		}
		else if (platform->getType() == Platform::Type::HalfLeft){
			makeHalfHorizontalColliders(world, platform, i, end);
		}
	}
}

void Room::makeEntity(TextureHolder &textureHolder, b2World &world, const int &num, const int &row, const int &col){
	int height = 50;
	int width = height;
	if (num > (int)Platform::EntityTrap::ArrowShooter && num < (int)Platform::EntityTrap::Torch)
	{
		std::shared_ptr<ArrowShooter> arrowShooter(new ArrowShooter());
		arrowShooter->init(textureHolder.get(Textures::ID::Square),
			sf::Vector2f(width * row, height * col),
			sf::IntRect(450, 0, 50, 50));
		arrowShooter->addBody(world);

		if (num == 14){
			arrowShooter->setFace(ArrowShooter::Face::West);
			arrowShooter->setTargetPos(sf::Vector2f(arrowShooter->getPos().x - 200, arrowShooter->getPos().y - 20));
		}
		else if (num == 15){
			arrowShooter->setFace(ArrowShooter::Face::North);
			arrowShooter->setTargetPos(sf::Vector2f(arrowShooter->getPos().x, arrowShooter->getPos().y - 200));
		}
		else if (num == 16){
			arrowShooter->setFace(ArrowShooter::Face::East);
			arrowShooter->setTargetPos(sf::Vector2f(arrowShooter->getPos().x + 200, arrowShooter->getPos().y - 20));
		}
		else{
			arrowShooter->setFace(ArrowShooter::Face::South);
			arrowShooter->setTargetPos(sf::Vector2f(arrowShooter->getPos().x, arrowShooter->getPos().y + 200));
		}

		mArrowShooters.push_back(arrowShooter);
	}
	else if (num < (int)Platform::EntityTrap::Enemy){
		
		std::shared_ptr<Torch> torch(new Torch());
		torch->init(textureHolder.get(Textures::ID::Torch), 
			sf::Vector2f(width * row, height * col), 
			sf::IntRect(0, 0, 64, 200));
		mTorches.push_back(std::move(torch));
	}
	else{
		std::unique_ptr<Enemy> enemy(new Slime());
		enemy->init(textureHolder, world, 
			sf::Vector2f(width * row, height * col));
		mEnemies.push_back(std::move(enemy));
	}
}

void Room::makeHorizontalColliders(b2World &world, std::shared_ptr<Platform> const &platform, std::vector<int>::iterator const &i, std::vector<int>::iterator const &end){
	const float scale = 0.033333333f; //use 0.033333f for mutiplecation instead of 30 for division.
	float blockWidth = 50;
	float blockHeight = 50;
	for (std::vector<int>::iterator j = i; j < end; j++){
		std::shared_ptr<Platform> nextPlatform = mPlatforms[*(j + 1)];
		sf::Vector2f startPos = platform->getPos();
		sf::Vector2f endPos = nextPlatform->getPos();
		if (startPos.y == endPos.y){
			if (nextPlatform->getType() == Platform::Type::Right || nextPlatform->getType() == Platform::Type::TopRight || nextPlatform->getType() == Platform::Type::BottomRight){
				float colliderWidth = endPos.x + blockWidth - startPos.x;
				b2Vec2 position = b2Vec2((startPos.x + endPos.x) * 0.5f * scale, startPos.y * scale);
				b2Vec2 size = b2Vec2(colliderWidth * 0.5f * scale, blockHeight * 0.5f * scale);
				addBody(world, "Wall", position, size);
				break;
			}
		}
	}
}

void Room::makeVerticalColliders(b2World &world, std::shared_ptr<Platform> const &platform, std::vector<int>::iterator const &i, std::vector<int>::iterator const &end){
	const float scale = 0.033333333f; //use 0.033333f for mutiplecation instead of 30 for division.
	float blockWidth = 50;
	for (std::vector<int>::iterator j = i; j < end; j++){
		std::shared_ptr<Platform> nextPlatform = mPlatforms[*(j + 1)];
		sf::Vector2f startPos = platform->getPos();
		sf::Vector2f endPos = nextPlatform->getPos();
		if (startPos.x == endPos.x){
			if (nextPlatform->getType() == Platform::Type::Bottom || nextPlatform->getType() == Platform::Type::BottomLeft || nextPlatform->getType() == Platform::Type::BottomRight){
				float colliderHeight = endPos.y + blockWidth - startPos.y;
				b2Vec2 position = b2Vec2(startPos.x * scale, (startPos.y + endPos.y) * 0.5f * scale);
				b2Vec2 size = b2Vec2(blockWidth * 0.5f * scale, colliderHeight * 0.5f * scale);
				addBody(world, "Wall", position, size);
				break;
			}
		}
	}
}

void Room::makeHalfHorizontalColliders(b2World &world, std::shared_ptr<Platform> const &platform, std::vector<int>::iterator const &i, std::vector<int>::iterator const &end){
	const float scale = 0.033333333f; //use 0.033333f for mutiplecation instead of 30 for division.
	float blockWidth = 50;
	for (std::vector<int>::iterator j = i; j < end; j++){
		std::shared_ptr<Platform> nextPlatform = mPlatforms[*(j + 1)];
		sf::Vector2f startPos = platform->getPos();
		sf::Vector2f endPos = nextPlatform->getPos();
		if (startPos.y == endPos.y){
			if (nextPlatform->getType() == Platform::Type::HalfRight){
				float colliderWidth = endPos.x + blockWidth - startPos.x;
				b2Vec2 position = b2Vec2((startPos.x + endPos.x) * 0.5f * scale, startPos.y * scale);
				b2Vec2 size = b2Vec2(colliderWidth * 0.5f * scale, 50 * 0.20f * 0.5f * scale);
				addBody(world, "Platform", position, size, -1);
				break;
			}
		}
	}
}

void Room::addBody(b2World &world, void* const &UserData, b2Vec2 const &position, b2Vec2 const &size, int const &groupIndex){
	b2BodyDef bodyDef;
	bodyDef.position = position;
	bodyDef.type = b2_staticBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x, size.y);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 100.f;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.shape = &shape;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(UserData);
}
void Room::update(float dt){
	for (auto &a : mTorches)
	{
		a->update(dt);
	}
}
void Room::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	
	for (auto &p : mEnemies)
	{
		target.draw(*p);
	}
	for (auto &p : mPlatforms)
	{
		target.draw(*p);
	}
	for (auto &p : mPlatformsHalf)
	{
		target.draw(*p);
	}
	for (auto &a : mArrowShooters)
	{
		target.draw(*a);
	}
	for (auto &a : mTorches)
	{
		target.draw(*a);
	}

}

void Room::setSize(const sf::Vector2i &size){
	mSize = size;
}

const sf::Vector2i & Room::getSize(){
	return mSize;
}
