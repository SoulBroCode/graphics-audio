#ifndef ROOM_H
#define ROOM_H

#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "Platform.h"
#include <vector>
#include <memory>
#include <iostream>
#include "ArrowShooter.h"
#include "Torch.h"
#include "Enemy.h"
#include "Slime.h"
class Room : public sf::Drawable, private sf::NonCopyable {
public:
	Room();
	bool makePlatformTile(const TextureHolder &, const int &, const int &, const int &);
	void makePlatformColliders(std::vector<int> &, b2World &world);
	bool makeHalfPlatform(const TextureHolder &, const int &, const int &, const int &);
	void makeEntity(TextureHolder &, b2World &, const int &, const int &, const int &);
	void update(float dt);
	void setSize(const sf::Vector2i &);
	const sf::Vector2i & getSize();
	std::vector<std::unique_ptr<Enemy>>	mEnemies;
	std::vector<std::shared_ptr<Torch>> mTorches;
	std::vector<std::shared_ptr<ArrowShooter>> mArrowShooters;
private:

	//methods
	void addBody(b2World &, void* const &, b2Vec2 const &, b2Vec2 const &, int const &groupIndex = 0);
	void makeHorizontalColliders(b2World &, std::shared_ptr<Platform> const &, std::vector<int>::iterator const &, std::vector<int>::iterator const &);
	void makeVerticalColliders(b2World &, std::shared_ptr<Platform> const &, std::vector<int>::iterator const &, std::vector<int>::iterator const &);
	void makeHalfHorizontalColliders(b2World &, std::shared_ptr<Platform> const &, std::vector<int>::iterator const &, std::vector<int>::iterator const &);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	//varibles	
	sf::Vector2i mSize;
	std::vector<std::shared_ptr<Platform>> mPlatforms;
	std::vector<std::shared_ptr<Platform>> mPlatformsHalf;

};
#endif