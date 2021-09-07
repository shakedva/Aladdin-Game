//----------------------------------include section-----------------------------------
#include "Enemy.h"

//----------------------------------class implementation------------------------------

Enemy::Enemy(sf::Vector2f pos, const AnimationData& data, const Aladdin& aladdin) :
	Movables(pos, data),
	m_enemyAction(EnemyAction::Idle),
	m_aladdin(aladdin)
{
	m_range = INIT;
	m_limTopLeft = m_sprite.getPosition();
	m_limBottomRight = m_sprite.getPosition();
}

//----------------------------------functions section---------------------------------

//convert enum class to int
int Enemy::enemyActionToInt(EnemyAction action)
{
	switch (action)
	{
	case EnemyAction::Idle:             return 0;
	case EnemyAction::Walk:             return 1;
	case EnemyAction::Attack:			return 2;
	case EnemyAction::Hit:			    return 3;
	default:                            break;
	}
	return 0;
}

//returns true if aladdin is in the same level as the enemy
bool Enemy::inEnemySight() const
{
	return (m_aladdin.getPosition().y > m_limBottomRight.y &&
		m_aladdin.getPosition().y < m_limTopLeft.y);
}

//changes the enemy's direction to face aladdin
void Enemy::lookAtAladdin()
{
	if (m_aladdin.getPosition().x < m_sprite.getPosition().x)
		setFaceDirection(true);
	else
		setFaceDirection(false);
}

//change the direction of the enemy according to where he goes
void Enemy::assumeWalking()
{
	if (m_sprite.getScale() == sf::Vector2f({ -1,1 }))	//Check prev direction of enemy
		setDirection(Direction::Right);
	else
		setDirection(Direction::Left);

	m_enemyAction = EnemyAction::Walk;
}

//make sure the enemy does'nt leave his boundaries
void Enemy::checkBoundaries()
{
	if (m_limTopLeft.x > m_sprite.getPosition().x)
		setFaceDirection(false);

	else if (m_limBottomRight.x < m_sprite.getPosition().x)
		setFaceDirection(true);
}

//----------------------------------set & get functions section------------------------

//set the sprite scale according to the enemy's direction
void Enemy::setFaceDirection(bool faceLeft)
{
	if (faceLeft)
	{
		m_sprite.setScale(RIGHT_SPRITE_TURN);
		setDirection(Direction::Left);
	}

	else if (!faceLeft)
	{
		m_sprite.setScale(LEFT_SPRITE_TURN);
		setDirection(Direction::Right);
	}
}

//return the direction of the enemy according to the sprite's scale
Direction Enemy::getDirection() const
{
	if (m_sprite.getScale() == RIGHT_SPRITE_TURN)
	{
		return Direction::Left;
	}
	else if (m_sprite.getScale() == LEFT_SPRITE_TURN)
	{
		return Direction::Right;
	}
	return Direction::Stay;
}
