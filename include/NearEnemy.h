//----------------------------------include section-----------------------------------
#pragma once
#include "Enemy.h"

//----------------------------------class definition----------------------------------
class NearEnemy : public Enemy
{
public:
	NearEnemy(sf::Vector2f, const Aladdin& aladdin, sf::Vector2f);
	virtual void updateAction(const sf::Time& deltaTime) override;
	virtual void setAction() override;


private:
	const float ATTACK_RANGE = 100;

	bool inAttackRange() const;
	bool inEnemyLines()  const;
	void handleInvasion();
};