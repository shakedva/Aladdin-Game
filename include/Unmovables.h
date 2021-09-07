//----------------------------------include section-----------------------------------
#pragma once
#include "GameObject.h"

//----------------------------------class definition----------------------------------
class Unmovables : public GameObject
{
public:
	using GameObject::GameObject;
	virtual ~Unmovables() = default;
	
	virtual void animateUnmovable(sf::Time delta) = 0;
};