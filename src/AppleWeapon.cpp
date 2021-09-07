
//----------------------------------include section-----------------------------------
#include "AppleWeapon.h"

//----------------------------------const section-----------------------------------
const int EXPLODE = 1;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
AppleWeapon::AppleWeapon(sf::Vector2f pos, Direction dir):
	Weapon(pos, Media::instance().animationData(Media::AppleWeapon))
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_aladdin));
	setDirection(dir);
}

void AppleWeapon::updateAction(const sf::Time& deltaTime)
{
	setAction();
	Movables::updateMovable(deltaTime, m_action);
}

//----------------------------------functions section---------------------------------

void AppleWeapon::createBody(b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody(world, position, size);
}

void AppleWeapon::setAction()
{
	if (m_dir == Direction::Stay)
	{
		if (m_animation.animationEneded() && m_action == EXPLODE)
			setDisposed();
		m_action = EXPLODE;
	}	
}