//----------------------------------Include Section---------------------------------

#include "DistantEnemy.h"

//----------------------------------const section-------------------------------------
const int IDLE_TIME = 4, WALK_TIME = 9, ATTACK_RNG_TIME = 1;
//----------------------------------Class Implementation------------------------------

DistantEnemy::DistantEnemy(sf::Vector2f pos, const Aladdin& aladdin, sf::Vector2f limit) :
	Enemy(pos, Media::instance().animationData(Media::DistantEnemy), aladdin)
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
void DistantEnemy::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	setAction();
	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * MOVE_SPEED, toVector(m_dir).y * MOVE_SPEED));
	Movables::updateMovable(deltaTime, enemyActionToInt(m_enemyAction));
	if (m_enemyAction == EnemyAction::Attack && m_elapsed.asSeconds() > SEC_LIM)
	{
		m_elapsed = m_elapsed.Zero;
		m_throwingWeapon = true;
	}
}

//set the enemy's action according to the time that has passed
void DistantEnemy::setAction()
{
	if (inEnemySight())

		if (inEnemyLines())	
		{
			lookAtAladdin();
			handleInvasion();
			return;
		}

	if (m_elapsed.asSeconds() < IDLE_TIME)
	{
		m_enemyAction = EnemyAction::Idle;
		setDirection(Direction::Stay);
	}
	else if (m_elapsed.asSeconds() < WALK_TIME)
	{
		assumeWalking(); 
		checkBoundaries();
	}
	else
		m_elapsed = m_elapsed.Zero;
}

//true if the player is in the same level as the enemy
bool DistantEnemy::inEnemyLines() const
{
	return (m_aladdin.getPosition().x < m_limBottomRight.x + ATTACK_RANGE &&
		m_aladdin.getPosition().x > m_limTopLeft.x - ATTACK_RANGE);
}

//change action if player entered a certain range
void DistantEnemy::handleInvasion()
{
	if (inAttackRange())
	{
		if (m_elapsed.asSeconds() > ATTACK_RNG_TIME)
		{
			m_enemyAction = EnemyAction::Attack;
			m_elapsed = m_elapsed.Zero;
			m_throwingWeapon = true;
		}

		setDirection(Direction::Stay);
	}
	else
		m_enemyAction = EnemyAction::Walk;
}

//true if aladdin's in a certain attack range
bool DistantEnemy::inAttackRange() const
{
	return (m_sprite.getPosition().x + ATTACK_RANGE >= m_aladdin.getPosition().x &&
		m_sprite.getPosition().x - ATTACK_RANGE <= m_aladdin.getPosition().x);
}
