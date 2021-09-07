//----------------------------------include section-----------------------------------
#pragma once
#include "Unmovables.h"

//----------------------------------class definition----------------------------------
class BaseLine: public Unmovables
{
public:
	BaseLine(sf::Vector2f, sf::Vector2f);

	virtual void draw(sf::RenderWindow&) const override {};
	virtual void animateUnmovable(sf::Time) override {};
	virtual void createBody(b2World*, sf::Vector2f, sf::Vector2f = {15,1}) override;

	virtual sf::FloatRect getGlobalBounds() const override;
	virtual sf::Vector2f getPosition() const override;
	
protected:
	sf::RectangleShape m_structureRect; //the structure's shape
};