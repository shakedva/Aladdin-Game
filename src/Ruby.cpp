//----------------------------------include section-------------------------------------
#include "Ruby.h"

//----------------------------------class implementation------------------------------

Ruby::Ruby(sf::Vector2f pos) : 
	ObjectsToCollect(pos, Media::instance().animationData(Media::Ruby))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_items));
}
