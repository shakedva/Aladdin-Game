//----------------------------------include section-----------------------------------

#include "LivesGift.h"

//----------------------------------class implementation------------------------------

LivesGift::LivesGift(sf::Vector2f pos) : 
	ObjectsToCollect(pos, Media::instance().animationData(Media::LivesGift))

{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_items));
}
