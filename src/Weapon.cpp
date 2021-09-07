//----------------------------------include section-----------------------------------
#include "Weapon.h"

//----------------------------------const section-----------------------------------

const int THROW_HEIGHT = -20, THROW_OFFSET = 40;
//----------------------------------class implementation------------------------------

Weapon::Weapon(sf::Vector2f pos, const AnimationData& data) 
	: Movables(pos, data)
{}

//----------------------------------functions section---------------------------------

void Weapon::updateAction(const sf::Time& deltaTime)
{
	Movables::updateMovable(deltaTime, INIT);
}

void Weapon::createBody(b2World* world, sf::Vector2f position , sf::Vector2f size)
{
	Movables::createBody(world, position, DEF_SIZE);

	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * THROW_OFFSET, THROW_HEIGHT));
}