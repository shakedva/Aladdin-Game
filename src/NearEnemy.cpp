//----------------------------------Include Section-----------------------------------

#include "NearEnemy.h"
#include <iostream> 

//----------------------------------Class Implementation------------------------------

NearEnemy::NearEnemy(sf::Vector2f pos, const Aladdin& aladdin, sf::Vector2f limit) :
	Enemy(pos, Media::instance().animationData(Media::NearEnemy), aladdin)
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_nonStaticEnemy));

	m_limTopLeft.x = limit.x;
	m_limBottomRight.x = limit.y;
	m_limBottomRight.y -= (aladdin.getGlobalBounds().height / 2);
	m_limTopLeft.y += (aladdin.getGlobalBounds().height / 2);
	setDirection(Direction::Right);
	setFaceDirection(false);
	m_enemyAction = EnemyAction::Walk;

}

//----------------------------------Functions Section---------------------------------

//update the enemy's action according to the time that passed
void NearEnemy::updateAction(const sf::Time& deltaTime)
{
	setAction();
	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * MOVE_SPEED, toVector(m_dir).y * MOVE_SPEED));
	Movables::updateMovable(deltaTime, enemyActionToInt(m_enemyAction));
}

//set the enemy's action according to the time that has passed
void NearEnemy::setAction()
{
	if (inEnemySight())
	{
		lookAtAladdin();

		if (inEnemyLines())
		{
			handleInvasion();
			return;
		}
		setDirection(Direction::Stay);
		m_enemyAction = EnemyAction::Idle;
	}

	else
	{
		assumeWalking();
		checkBoundaries();
	}
}

//true if aladdin's in a certain attack range
bool NearEnemy::inAttackRange() const
{
	return (m_sprite.getPosition().x + ATTACK_RANGE >= m_aladdin.getPosition().x &&
		m_sprite.getPosition().x - ATTACK_RANGE <= m_aladdin.getPosition().x);
}

//true if the player is in the same level as the enemy
bool NearEnemy::inEnemyLines() const
{
	return (m_aladdin.getPosition().x < m_limBottomRight.x&&
		m_aladdin.getPosition().x > m_limTopLeft.x);
}

//change action if player entered a certain range
void NearEnemy::handleInvasion()
{
	if (inAttackRange())
	{
		m_enemyAction = EnemyAction::Attack;
		setDirection(Direction::Stay);
	}
	else
		m_enemyAction = EnemyAction::Walk;
}



