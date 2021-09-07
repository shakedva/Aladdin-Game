//----------------------------------include section-----------------------------------
#pragma once
#include "Unmovables.h"

//----------------------------------class definition----------------------------------
class ObjectsToCollect : public Unmovables
{
public:
	ObjectsToCollect(sf::Vector2f, const AnimationData& data);

	virtual void createBody(b2World*, sf::Vector2f, sf::Vector2f = { 15,1 }) {};
	virtual void animateUnmovable(sf::Time delta) override;

protected:
	Animation m_animation;  // the current animation of the object
};