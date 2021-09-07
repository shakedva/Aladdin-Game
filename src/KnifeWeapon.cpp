//----------------------------------Include Section---------------------------------

#include "KnifeWeapon.h"

//----------------------------------Class Implementation------------------------------

KnifeWeapon::KnifeWeapon(sf::Vector2f pos, Direction dir) :
	Weapon(pos, Media::instance().animationData(Media::KnifeWeapon))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_nonStaticEnemy));
	setDirection(dir);
}

void KnifeWeapon::createBody(b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody(world, position, size);
}

