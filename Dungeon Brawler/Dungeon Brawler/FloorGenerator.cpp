#include "FloorGenerator.h"

FloorGenerator::FloorGenerator(){}
void FloorGenerator::loadFile(const char * filename){
	FILE* fp = fopen(filename, "rb");
	FileStream is(fp);
	mDocument.ParseStream<0>(is);
}
void FloorGenerator::makeRooms(TextureHolder & textureHolder, b2World &world){
	for (int i = 0; i < 1; i++)
	{
		mRooms.push_back(std::make_shared<Room>());
	}
	const Value& layer = mDocument["layers"];
	for (std::shared_ptr<Room> &room : mRooms)
	{
		makePlatform(room, layer, textureHolder, world);
		makeEntity(room, layer, textureHolder, world);
		//room->callEntity(world);
	}
	mCurrentRoom = mRooms[0];
}

void FloorGenerator::makePlatform(std::shared_ptr<Room> &room, const Value& layer, TextureHolder & textureHolder, b2World &world){
	std::vector<int> platfromCornerIndex;			//vector of index where there is a corner	
	std::vector<int> platfromHalfCornerIndex;
	const Value& platformLayer = layer[0u];
	int row = platformLayer["width"].GetInt();
	int col = platformLayer["height"].GetInt();
	room->setSize(sf::Vector2i(row, col));
	int platformCurrentIndex = 0;					//the actual platform index count
	int dataCurrentIndex = 0;						//keep track of the index count to read from json
	for (int j = 0; j < col; j++){
		for (int i = 0; i < row; i++){
			int dataType = platformLayer["data"][dataCurrentIndex].GetInt();
			if (dataType != (int)Platform::Type::Null){//checking if it is a tile

				if (room->makePlatformTile(textureHolder, dataType, i, j)){
					platfromCornerIndex.push_back(platformCurrentIndex);
				}
				platformCurrentIndex++;

			}
			dataCurrentIndex++;
		}
	}
	room->makePlatformColliders(platfromCornerIndex, world);
	dataCurrentIndex = 0;
	platformCurrentIndex = 0;
}

void FloorGenerator::makeEntity(std::shared_ptr<Room> &room, const Value& layer, TextureHolder & textureHolder, b2World &world){
	const Value& entityLayer = layer[1];
	int row = room->getSize().x;
	int col = room->getSize().y;
	int dataCurrentIndex = 0;	//keep track of the index count to read from json
	for (int j = 0; j < col; j++){
		for (int i = 0; i < row; i++){
			int dataType = entityLayer["data"][dataCurrentIndex].GetInt();
			if (dataType != (int)Platform::Type::Null){	//checking if it is a tile
				room->makeEntity(textureHolder, world, dataType, i, j);
			}
			dataCurrentIndex++;
		}
	}
	dataCurrentIndex = 0;
}

std::shared_ptr<Room> & FloorGenerator::GetCurrentRoom(){
	return mCurrentRoom;
}
void FloorGenerator::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	target.draw(*mCurrentRoom);
}

