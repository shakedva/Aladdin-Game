//----------------------------------include section-----------------------------------
#include "Aladdin.h"
#include "Media.h"

//----------------------------------const section-------------------------------------

const float OFFSET = 0.5;
constexpr auto RUNNIG_SPEED = 30.f,
				CLIMING_SPEED = 10.f,
				JUMP_IMPULSE = 100.f,
				JUMP_IMPULSE_ATTACK = 2.f ,
				JUMP_IMPULSE_IDLE = 3.f,
				JUMP_IMPULSE_RUN = 1.5f,
				JUMP_IMPULSE_THROW = 2.f ;
const int THROW_TIME = 1;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
Aladdin::Aladdin(sf::Vector2f pos) : Movables(pos, Media::instance().animationData(Media::Aladdin)),
									m_action(PlayerAction::Idle), m_animationAction(AnimationAction::Idle)
{
	m_aladdinStatus.resetAladdinStatus();
	m_playerData._score = SCORE_INIT;
	m_playerData._lives = LIVES_DEFAULT;
	m_playerData._weapons = WEAPON_INIT;
	m_sprite.setTexture(Media::instance().getTexture(Media::t_aladdin));
}

//----------------------------------functions section---------------------------------

Direction Aladdin::dirFromKey() const
{
	using KeyMapping = std::initializer_list<std::pair<sf::Keyboard::Key, Direction>>;
	static const KeyMapping keyToDirectionMapping =
	{
		{ sf::Keyboard::Right, Direction::Right},
		{ sf::Keyboard::Left , Direction::Left },
		{ sf::Keyboard::Up   , Direction::Up   },
		{ sf::Keyboard::Down , Direction::Down },
	};

	for (const auto& pair : keyToDirectionMapping)
		if (sf::Keyboard::isKeyPressed(pair.first))
			return pair.second;

	return Direction::Stay;
}

//update aladdin's cur action
void Aladdin::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	if ((m_action == PlayerAction::Idle) ||
		(m_action != PlayerAction::Idle &&
			m_animation.animationEneded()))
	{
		setAction();
		m_aladdinStatus._onRope = false;
		m_aladdinStatus._onGround = false;
		m_aladdinStatus._isFalling = false;
	}
	m_sprite.setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
	if (m_action != PlayerAction::Idle)
		m_animation.speedUpAnimation(true);
	Movables::updateMovable(deltaTime, playerActionToInt(m_animationAction));
}

// update aladdin's data
void Aladdin::updateInfo(int feature, int amountToAdd)
{
	switch (feature)
	{
	case SCORE:
		m_playerData._score += amountToAdd;
		break;
	case WEAPONS:
		m_playerData._weapons += amountToAdd;
		break;
	case LIVES:
		m_playerData._lives += amountToAdd;
		break;
	}
}

//all of aladdin's possible actions
int Aladdin::playerActionToInt(AnimationAction action) const
{
	switch (action)
	{
	case AnimationAction::Idle:				return 0;
	case AnimationAction::Run:				return 1;
	case AnimationAction::Attack:			return 2;
	case AnimationAction::Throw:			return 3;
	case AnimationAction::ClimbVertical:	return 4;
	case AnimationAction::IdleOnRope:		return 5;
	case AnimationAction::AttackOnRope:		return 6;
	case AnimationAction::ThrowOnRope:		return 7;
	case AnimationAction::IdleJump:			return 8;
	case AnimationAction::RunJump:			return 9;
	case AnimationAction::JumpThrow:		return 10;
	case AnimationAction::JumpAttack:		return 11;
	case AnimationAction::Fall:				return 12;
	case AnimationAction::FallFromRope:     return 13;
	case AnimationAction::Win:				return 14;
	case AnimationAction::Crouch:			return 15;
	case AnimationAction::CrouchAttack:		return 16;
	case AnimationAction::CrouchThrow:		return 17;
	default:								break;
	}
	return 0;
}

//being called in each restart of the game
void Aladdin::resetAladdinData()
{
	m_playerData._lives = LIVES_DEFAULT;
	m_playerData._score = SCORE_INIT;
	m_playerData._weapons = WEAPON_INIT;
	m_defualtPosition = convertVec2fTob2Vec2(STAY_VEC);
	m_aladdinStatus._onRope = false;	
	m_aladdinStatus._onGround = false;
	m_aladdinStatus._isJumping = false;
	m_aladdinStatus._isFalling = true;  
}

//move aladdin a little backwards when hitting a wall
void Aladdin::hitAwall()
{
	sf::Vector2f mv; 
	if (getDirection() == Direction::Right)
		mv.x = -OFFSET + m_body->GetPosition().x;
	else if (getDirection() == Direction::Left)
		mv.x = OFFSET + m_body->GetPosition().x;

	mv.y = m_body->GetPosition().y;
	m_body->SetTransform(convertVec2fTob2Vec2(mv), m_body->GetAngle());
	m_sprite.setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
}

//center aladdin when climbing on a rope
void Aladdin::centerOnRope(sf::Vector2f pos)
{
	if (!m_aladdinStatus._isJumping && !m_aladdinStatus._onGround)
		m_body->SetTransform({ convertVec2fTob2Vec2(pos).x, m_sprite.getPosition().y }, m_body->GetAngle());
	b2Vec2 position = convertVec2fTob2Vec2(pos); 
}

//attach a key to an action of aladdin
Aladdin::PlayerAction Aladdin::actionFromKey() const
{
	using KeyMapping = std::initializer_list<std::pair<sf::Keyboard::Key, PlayerAction>>;
	static const KeyMapping keyToactionMapping =
	{
		{ sf::Keyboard::LControl, PlayerAction::Attack},
		{ sf::Keyboard::LAlt    , PlayerAction::Throw },
		{ sf::Keyboard::X		, PlayerAction::Attack},
		{ sf::Keyboard::Z		, PlayerAction::Throw },
	};

	for (const auto& pair : keyToactionMapping)
		if (sf::Keyboard::isKeyPressed(pair.first))
			return pair.second;

	return PlayerAction::Idle;
}

// set scale according to the pressed direction
void Aladdin::faceToCorrectDirection(Direction dir)
{
	if (m_dir != dir && dir == Direction::Left)
		m_sprite.setScale(LEFT_SPRITE_TURN);
	else if (m_dir != dir && dir == Direction::Right)
		m_sprite.setScale(RIGHT_SPRITE_TURN);
	m_dir = dir;
}

//update aladdin's AnimationAction while in jump mode
b2Vec2 Aladdin::handleJump()
{
	b2Vec2 jumpForce(0, -1);
	if (!m_aladdinStatus._isJumping && (m_aladdinStatus._onRope || m_aladdinStatus._onGround))
	{
		m_aladdinStatus._isJumping = true;
		jumpForce.y = -1;
		switch (m_animationAction)
		{
		case Aladdin::AnimationAction::Idle:
			Media::instance().playSound(Media::s_jump);
			m_animationAction = AnimationAction::IdleJump;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_IDLE;
			break;

		case Aladdin::AnimationAction::Run:
			Media::instance().playSound(Media::s_jump);
			m_animationAction = AnimationAction::RunJump;
			m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x / 2, m_body->GetLinearVelocity().y));
			jumpForce *= JUMP_IMPULSE * JUMP_IMPULSE_RUN;
			break;

		case Aladdin::AnimationAction::Attack:
			Media::instance().playSound(Media::s_jump);
			m_animationAction = AnimationAction::JumpAttack;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_ATTACK;
			break;

		case Aladdin::AnimationAction::Throw:
			Media::instance().playSound(Media::s_throwApp);
			m_animationAction = AnimationAction::JumpThrow;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_THROW;
			break;

		case Aladdin::AnimationAction::IdleOnRope:
			Media::instance().playSound(Media::s_jump);
			m_animationAction = AnimationAction::FallFromRope;
			jumpForce *= JUMP_IMPULSE;
			m_body->SetLinearVelocity(b2Vec2(toVector(dirFromKey()).x * CLIMING_SPEED, 0));
			break;
		default:
			break;
		}
	}
	else
	{
		m_action = actionFromKey();
		switch (m_action)
		{
		case Aladdin::PlayerAction::Idle:
			break;
		case Aladdin::PlayerAction::Attack:
			m_animationAction = AnimationAction::JumpAttack;
			break;
		case Aladdin::PlayerAction::Throw:
			m_animationAction = AnimationAction::JumpThrow;
			break;
		default:
			break;
		}
	}
	return jumpForce;
}

//update aladdin's AnimationAction while in walking mode
void Aladdin::handleMove()
{
	b2Vec2 moveBodyB2ve2(0, 1); //temp variable

	if (m_aladdinStatus._onGround)
	{
		m_aladdinStatus._isJumping = false;
		if (m_dir == Direction::Right || m_dir == Direction::Left)
		{
			m_animationAction = AnimationAction::Run;
			moveBodyB2ve2 = convertVec2fTob2Vec2(toVector(m_dir) * RUNNIG_SPEED);
			m_body->SetLinearVelocity(moveBodyB2ve2);
		}
		else if (m_dir == Direction::Down)
			m_animationAction = AnimationAction::Crouch;
		else
			m_animationAction = AnimationAction::Idle;
	}
	else if (m_aladdinStatus._onRope)
	{
		m_aladdinStatus._isJumping = false;
		if (m_dir == Direction::Up || m_dir == Direction::Down)
		{
			m_animationAction = AnimationAction::ClimbVertical;
			moveBodyB2ve2 = convertVec2fTob2Vec2(toVector(m_dir) * CLIMING_SPEED);
			m_body->SetLinearVelocity(moveBodyB2ve2);
		}
		else
			m_animationAction = AnimationAction::IdleOnRope;
	}
	else if (m_aladdinStatus._isJumping && m_animation.animationEneded() && m_action == PlayerAction::Idle)
	{
		m_aladdinStatus._isJumping = false;
		m_animationAction = AnimationAction::Fall;
	}
}

//update aladdin's current action according to a key received or his last status
void Aladdin::handleAct()
{
	m_action = actionFromKey();
	if (m_action == PlayerAction::Attack)
	{
		switch (m_animationAction)
		{
		case Aladdin::AnimationAction::Idle:
			m_animationAction = AnimationAction::Attack;
			Media::instance().playSound(Media::s_aliSword);
			break;
		case Aladdin::AnimationAction::IdleOnRope:
			m_animationAction = AnimationAction::AttackOnRope;
			Media::instance().playSound(Media::s_aliSword);
			break;
		case Aladdin::AnimationAction::Crouch:
			Media::instance().playSound(Media::s_aliSword);
			m_animationAction = AnimationAction::CrouchAttack;
			m_dir = Direction::Stay;
			break;
		default:
			break;
		}
	}
	else if (m_action == PlayerAction::Throw)
	{
		if (m_action == PlayerAction::Throw && m_elapsed.asSeconds() > THROW_TIME)
		{
			m_elapsed = m_elapsed.Zero;
			m_throwingWeapon = true;

			switch (m_animationAction)
			{
			case Aladdin::AnimationAction::Idle:
				Media::instance().playSound(Media::s_throwApp);
				m_animationAction = AnimationAction::Throw;
				break;
			case Aladdin::AnimationAction::IdleOnRope:
				Media::instance().playSound(Media::s_throwApp);
				m_animationAction = AnimationAction::ThrowOnRope;
				break;
			case Aladdin::AnimationAction::Crouch:
				Media::instance().playSound(Media::s_throwApp);
				m_animationAction = AnimationAction::CrouchThrow;
				m_dir = Direction::Stay;
				break;
			}
		}
	}
}

//limit aladdin while reaching to the top of a rope
void Aladdin::reachedEndOfRope()
{
	m_body->SetTransform({ m_body->GetPosition().x, m_body->GetPosition().y + OFFSET }, m_body->GetAngle());
}

//----------------------------------set functions section---------------------------------

void Aladdin::setAction()
{
	//Get player direction from keyboard
	auto dir = dirFromKey();
	// set scale according to the pressed direction
	faceToCorrectDirection(dir);

	handleMove();
	handleAct();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_body->ApplyLinearImpulse(handleJump(), m_body->GetPosition(), true);

	if (m_animationAction == AnimationAction::IdleOnRope)
		m_body->SetAwake(false);
	if (m_aladdinStatus._isJumping)
		m_body->SetAwake(true);
}

//if aladdin lost life, he returns to his default position and his status is reset
void Aladdin::setAladdinToDefaultPosition()
{
	m_body->SetTransform(m_defualtPosition, m_body->GetAngle());
	m_aladdinStatus.resetAladdinStatus();
	m_body->SetAwake(true);
	b2Vec2 position = m_body->GetPosition();
	m_sprite.setPosition(position.x, position.y);
}

void Aladdin::setOnGround(bool onGround) { m_aladdinStatus._onGround = onGround; }

void Aladdin::setOnRope(bool onRope) { m_aladdinStatus._onRope = onRope; }

void Aladdin::setIdleAction() { m_action = PlayerAction::Idle; }

void Aladdin::setDefaultPosition(sf::Vector2f pos) { m_defualtPosition = convertVec2fTob2Vec2(pos); }

void Aladdin::setSpeed(float speed) { m_playerData._speed = speed; }

//----------------------------------get functions section---------------------------------

const PlayerData& Aladdin::getAladdinData() const {	return m_playerData; }

bool Aladdin::getOnRope() const { return m_aladdinStatus._onRope; }

sf::Vector2f Aladdin::getAladdinPos() const { return m_sprite.getPosition(); }

Aladdin::PlayerAction Aladdin::getAction() const { return m_action; }