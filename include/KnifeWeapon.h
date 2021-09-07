//----------------------------------include section-----------------------------------
#pragma once
#include "Weapon.h"

//----------------------------------class definition----------------------------------
class KnifeWeapon : public Weapon
{
public:
	KnifeWeapon(sf::Vector2f, Direction);
	virtual void createBody(b2World*, sf::Vector2f, sf::Vector2f = { 1,1 }) override;
};