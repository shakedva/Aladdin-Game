//----------------------------------include section-----------------------------------
#pragma once
#include "GameObject.h"
#include "Direction.h"

//----------------------------------class definition----------------------------------
class Movables : public GameObject
{
public:
	Movables(sf::Vector2f, const AnimationData&);

	virtual void setAction() = 0;
	virtual void updateAction(const sf::Time&) = 0;
	virtual void createBody(b2World*, sf::Vector2f, sf::Vector2f = {15,1}) override;

	bool isThrowingWeapon() const;
	virtual Direction getDirection() const;
	void setDirection(Direction);
	void setThrowingWeapon(bool throwing);	

protected:
	virtual void updateMovable(sf::Time, int); 

	Animation       m_animation;  // current animation of the object
	Direction		m_dir = Direction::Stay;  // current direction of the object
	bool			m_throwingWeapon = false; // if the object is currently throwing a weapon
	sf::Time		m_elapsed = {};     // the time that passed since the last time we checked
};