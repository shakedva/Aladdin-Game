//----------------------------------include section-----------------------------------
#pragma once
#include "Movables.h"

//----------------------------------const section-------------------------------------
const float weaponSpeed = 400.f;
const sf::Vector2f DEF_SIZE = { 1,1 };

//----------------------------------class definition----------------------------------
class Weapon : public Movables
{
public:
	Weapon(sf::Vector2f, const AnimationData&);
	virtual void updateAction(const sf::Time&) override;
	virtual void createBody(b2World*, sf::Vector2f, sf::Vector2f = { 15,1 }) override;
	virtual void setAction()override {};
};