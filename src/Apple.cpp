  //----------------------------------include section-------------------------------------
#include "Apple.h"

//----------------------------------class implementation------------------------------

Apple::Apple(sf::Vector2f pos) : 
	ObjectsToCollect(pos, Media::instance().animationData(Media::Apple))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_items));
}

