//----------------------------------include section-----------------------------------
#include "EndLevel.h"

//----------------------------------functions section---------------------------------

EndLevel::EndLevel(sf::Vector2f pos) :
	ObjectsToCollect(pos, Media::instance().animationData(Media::EndLevel))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_endLevel));
}
