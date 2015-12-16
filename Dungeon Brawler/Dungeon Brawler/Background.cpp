#include "Background.h"

Background::Background(){

}
void Background::create(TextureHolder & textureHolder,const sf::Vector2f &gameWorldSize){
	sf::Sprite spriteTemp;
	int width = 100;
	int height = width;
	const float scale = 30.f;
	for (int y = -2; y < gameWorldSize.y; y++){
		for (int x = -5; x < gameWorldSize.x; x++){
			spriteTemp.setPosition(x * 100 -25, y * 100-25);
			spriteTemp.setColor(sf::Color(180, 180, 180));
			spriteTemp.setTexture(textureHolder.get(Textures::ID::Tilemap));
			int rndNum = rand() % 5;
			sf::IntRect platformTextureRec;
			switch (rndNum) {
			case 0:
				platformTextureRec = sf::IntRect(400, 0, width, height);
				break;
			case 1:
				platformTextureRec = sf::IntRect(0, 100, width, height);
				break;
			case 2:
				platformTextureRec = sf::IntRect(100, 100, width, height);
				break;
			case 3:
				platformTextureRec = sf::IntRect(200, 100, width, height);
				break;
			default:
				platformTextureRec = sf::IntRect(300, 100, width, height);
				break;
			}
			spriteTemp.setTextureRect(platformTextureRec);
			//spriteTemp.setScale(sf::Vector2f(0.5f, 0.5f));
			mSprite.push_back(spriteTemp);
		}
	}

}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	for (auto &s: mSprite){
		target.draw(s,states);
	}

}

