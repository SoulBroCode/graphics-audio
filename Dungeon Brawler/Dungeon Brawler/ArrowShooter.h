#ifndef ARROWSHOOTER_H
#define ARROWSHOOTER_H
#include "SFML\Graphics.hpp"
#include <vector>
#include <memory>
#include "RangeWeapon.h";

class ArrowShooter : public sf::Drawable {
public:
	enum class Face
	{
		West, North, South, East,
	};//clockwise
	ArrowShooter();
	void init(sf::Texture const &, sf::Vector2f const &, sf::IntRect const &);
	void addBody(b2World &world);
	void update(float dt);
	void setFace(Face face);
	void setFireDelay(float const& delay);
	void setTargetPos(sf::Vector2f const &);
	bool getCanFire();
	void setCanFire(bool);
	bool getPauseSound();
	void setPauseSound(bool);
	const float & getFireDelay();
	sf::Vector2f const & getPos();
	std::unique_ptr<Projectile> createProjectile(float pwrForce);

	const sf::FloatRect & getLineOfSight();

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	sf::Sprite mSprite;
	sf::FloatRect mLineOfSight;
	bool mCanFire;
	bool mPauseSound;
	b2Body*	mBody;
	sf::Vector2f mTargetPos;
	sf::Vector2f mArrowSpawnPos;
	float mFireDelay;
	RangeWeapon mRangedWeapon;
	FMOD::Channel *mChannel;


};
#endif