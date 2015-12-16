#include "Platform.h"

Platform::Platform(){}
void Platform::init(sf::Texture const &texture, sf::Vector2f const &position, sf::IntRect const &spriteRect){
	mSprite.setTexture(texture);
	mSprite.setPosition(position);
	mSprite.setTextureRect(spriteRect);
	mSprite.setOrigin(spriteRect.width * 0.5f, spriteRect.height * 0.5f);
}
void Platform::setOrigin(sf::Vector2f const &origin){
	mSprite.setOrigin(origin);
}
void Platform::draw(sf::RenderTarget& target, sf::RenderStates states)  const{
	target.draw(mSprite, states);
}
static Platform * create(sf::Vector2f position){

}
void Platform::setType(Platform::Type const &type){
	mType = type;
}

sf::Vector2f const & Platform::getPos(){
	return mSprite.getPosition();
}
Platform::Type const &  Platform::getType(){
	return mType;
}
