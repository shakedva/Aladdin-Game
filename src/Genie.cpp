//----------------------------------include section-----------------------------------
#include "Genie.h"

//----------------------------------class implementation------------------------------

Genie::Genie(sf::Vector2f pos) : 
	ObjectsToCollect(pos, Media::instance().animationData(Media::Genie))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_items));
}
