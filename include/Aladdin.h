//----------------------------------include section-----------------------------------
#pragma once
#include <optional>
#include "Movables.h"
#include "AppleWeapon.h"
#include "box2d/box2d.h"

//----------------------------------class definition----------------------------------
class Aladdin : public Movables
{
public:
	Aladdin(sf::Vector2f);
	
	enum class PlayerAction
	{
		Idle,										
		Attack,
		Throw
	
	};
	
	enum class AnimationAction
	{
		Idle,					// 0
		Run,					// 1
		Attack,                 // 2		
		Throw,				    // 3
		ClimbVertical,			// 4
		IdleOnRope,				// 5
		AttackOnRope,			// 6
		ThrowOnRope,			// 7
		IdleJump,				// 8
		RunJump,				// 9 --> needs two keys
		JumpThrow,				// 10 --> needs two keys
		JumpAttack,				// 11 --> needs two keys
		Fall,					// 12
		FallFromRope,			// 13
		Win,					// 14 --> collision with end level item
		Crouch,					// 15
		CrouchAttack,			// 16
		CrouchThrow,			// 17
	};

	virtual void updateAction(const sf::Time&) override;
	virtual void setAction() override;

	void hitAwall();
	void updateInfo(int, int);
	void centerOnRope(sf::Vector2f);
	void resetAladdinData();
	void reachedEndOfRope();
	int playerActionToInt(AnimationAction action) const;


	sf::Vector2f getAladdinPos() const;
	PlayerAction getAction() const;
	const PlayerData& getAladdinData() const;
	bool getOnRope() const;

	void setOnGround(bool);
	void setOnRope(bool);
	void setSpeed(float);
	void setIdleAction();
	void setAladdinToDefaultPosition();
	void setDefaultPosition(sf::Vector2f);

private:
	Direction dirFromKey() const;
	Aladdin::PlayerAction actionFromKey() const;
	void faceToCorrectDirection(Direction);  
	b2Vec2 handleJump();
	void handleMove();
	void handleAct();
	
	struct AladdinStatus
	{
		bool _onRope,   // if the player is currently on a rope
			_wonLevel,  // if the player won the level
			_onGround,  // if the player is on the ground
			_isJumping, // if the player is jumping
			_isFalling;  // if the player is falling
		void resetAladdinStatus()
		{
			_onRope = false;
			_wonLevel = false;
			_onGround = false;
			_isJumping = false;
			_isFalling = true;
		}
	};

	PlayerData		m_playerData;       // the player's score, lives and weapons
	PlayerAction	m_action;	        // the player's current action
	b2Vec2			m_defualtPosition;  // the player's default position
	AnimationAction m_animationAction;  // the player's current animation action
	AladdinStatus   m_aladdinStatus;	// the player's status with other objects
	sf::Time		m_deltaTime = {};   // current time that passed


	
};
