//----------------------------------Include Section-----------------------------------

#include "JugglingEnemy.h"

//----------------------------------Class Implementation------------------------------


JugglingEnemy::JugglingEnemy(sf::Vector2f pos, const Aladdin& aladdin) :
	Enemy(pos, Media::instance().animationData(Media::JugglingEnemy), aladdin)
{
	m_sprite.setTexture(Media::instance().getTexture(Media::t_staticEnemy));

	m_limBottomRight.x = m_sprite.getPosition().x;
	m_limTopLeft.x = m_sprite.getPosition().x;
	m_limBottomRight.y -= (aladdin.getGlobalBounds().height / 2);
	m_limTopLeft.y += (aladdin.getGlobalBounds().height / 2);
	setDirection(Direction::Stay);
}

//update the enemy's action according to the time that passed
void JugglingEnemy::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	setAction();
	Movables::updateMovable(deltaTime, enemyActionToInt(EnemyAction::Idle));
}

Direction JugglingEnemy::getDirection() const
{
	return Movables::getDirection();
}

//true if the player is in the same level as the enemy
bool JugglingEnemy::inEnemyLines() const
{
	return (m_aladdin.getPosition().x < m_sprite.getPosition().x + ATTACK_RANGE &&
		m_aladdin.getPosition().x > m_sprite.getPosition().x - ATTACK_RANGE);
}

//change action if player entered a certain range
void JugglingEnemy::handleInvasion()
{
	if (m_elapsed.asSeconds() > SEC_LIM)
	{
		m_elapsed = m_elapsed.Zero;
		m_throwingWeapon = true;
	}
}

//set the enemy's action according to the time that has passed
void JugglingEnemy::setAction()
{
	if (inEnemySight())	
		if (inEnemyLines())
		{
			lookAtAladdin();
			handleInvasion();
		}
}

//change the direction of the sprite to face aladdin when aladdin gets close to him
void JugglingEnemy::lookAtAladdin()
{
	if (m_aladdin.getPosition().x < m_sprite.getPosition().x)
	{
		m_sprite.setScale(LEFT_SPRITE_TURN);
		setDirection(Direction::Left);
	}
	else
	{
		m_sprite.setScale(RIGHT_SPRITE_TURN);
		setDirection(Direction::Right);
	}
}