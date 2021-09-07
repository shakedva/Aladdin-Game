//----------------------------------include section-------------------------------------
#include "ObjectsToCollect.h"

//----------------------------------class implementation------------------------------

ObjectsToCollect::ObjectsToCollect(sf::Vector2f pos, const AnimationData& data):
	Unmovables(pos), m_animation(data, m_sprite) {}

//----------------------------------function section-------------------------------------

void ObjectsToCollect::animateUnmovable(sf::Time delta)
{
	m_animation.update(delta);
}

