//----------------------------------include section-----------------------------------
#pragma once
#include "Enemy.h"

//----------------------------------class definition----------------------------------
class DistantEnemy : public Enemy
{
public:
	DistantEnemy(sf::Vector2f, const Aladdin&, sf::Vector2f);
	virtual void setAction() override;
	virtual void updateAction(const sf::Time&) override;


private:
	const float ATTACK_RANGE = 150;
	sf::Time		m_elapsed = {};     // the time that passed since the last time we checked

	bool inAttackRange() const;
	bool inEnemyLines()  const;
	void handleInvasion();
};