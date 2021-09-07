//----------------------------------include section-----------------------------------
#pragma once
#include "Enemy.h"

//----------------------------------class definition----------------------------------
class JugglingEnemy : public Enemy
{
public:
	JugglingEnemy(sf::Vector2f, const Aladdin& aladdin);
	virtual void setAction() override;
	virtual void updateAction(const sf::Time&) override;
	virtual Direction getDirection() const override; // Additional override for Juggling because is facing same as Aladdin

private:
	const float ATTACK_RANGE = 150;

	bool inEnemyLines() const;
	void handleInvasion();
	void lookAtAladdin();
};