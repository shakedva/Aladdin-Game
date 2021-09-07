//----------------------------------include section-----------------------------------
#include "BaseLine.h"

//----------------------------------class implementation------------------------------
BaseLine::BaseLine(sf::Vector2f coords, sf::Vector2f size) : m_structureRect(size)
{

	m_structureRect.setOrigin(size.x / 2, size.y / 2);
	m_structureRect.setPosition(coords);
	m_structureRect.setOutlineThickness(2);
	m_structureRect.setOutlineColor(sf::Color::Transparent);
	m_structureRect.setFillColor(sf::Color::Transparent);
}

//----------------------------------functions section---------------------------------


//create a ground body to all static objects
void BaseLine::createBody(b2World* world, sf::Vector2f pos, sf::Vector2f size)
{
	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;

	groundBodyDef.position.Set(pos.x, pos.y);

	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	groundBox.SetAsBox(size.x / 2, size.y / 2);
	auto fix = groundBody->CreateFixture(&groundBox, 0.0f);
	fix->SetFriction(DENS);
	fix->SetDensity(FRIC);
	m_body = groundBody;
}

sf::FloatRect BaseLine::getGlobalBounds() const
{
	return m_structureRect.getGlobalBounds(); 
}

sf::Vector2f BaseLine::getPosition() const
{
	return m_structureRect.getPosition();
}
