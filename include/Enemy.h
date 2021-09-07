//----------------------------------include section-----------------------------------
#pragma once
#include "Movables.h"
#include "Aladdin.h"

//----------------------------------const section-------------------------------------
const int LIM = 200;
const float SEC_LIM = 1.5;
//----------------------------------class definition----------------------------------
class Enemy : public Movables
{
public:
	enum class EnemyAction
	{
		Idle,					
		Walk,                  
		Attack,                 
		Hit                     
	};

	Enemy(sf::Vector2f, const AnimationData&, const Aladdin&);

	virtual Direction getDirection() const override;
	int enemyActionToInt(EnemyAction); 

protected: 
	//Protected Funcs
	void setFaceDirection(bool);
	bool inEnemySight() const;
	void lookAtAladdin();
	void assumeWalking();
	void checkBoundaries();

	//Protected Members
	const float		MOVE_SPEED = 15.f;
	EnemyAction     m_enemyAction;    // Enemy current action
	sf::Vector2f	m_limTopLeft,     // distance limit from the left
					m_limBottomRight; // distance limit from the right
	float			m_range;		  //the range in which the enemy can move around
	const Aladdin&  m_aladdin;		  //the enemy knows aladdin
};
