#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#define _CRT_SECURE_NO_DEPRECATE
#include "Room.h"
#include "Platform.h"
#include <vector>
#include <memory>

#include "rapidjson\document.h"
#include "rapidjson\filestream.h"
#include <cstdio>


using namespace rapidjson;

class FloorGenerator : public sf::Drawable, private sf::NonCopyable {
public:
	FloorGenerator();
	void loadFile(const char * filename);
	void makeRooms(TextureHolder &, b2World &);
	std::shared_ptr<Room> & GetCurrentRoom();

private:
	//methods
	void makePlatform(std::shared_ptr<Room> &room, const Value& layer,TextureHolder & textureHolder, b2World &world);
	void makeEntity(std::shared_ptr<Room> &room, const Value& layer, TextureHolder & textureHolder, b2World &world);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	//varibles
	Document mDocument;
	std::vector<std::shared_ptr<Room>> mRooms;
	std::shared_ptr<Room> mCurrentRoom;
};
#endif