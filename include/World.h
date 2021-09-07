//----------------------------------include section-----------------------------------
#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "Movables.h"
#include "Unmovables.h"
#include "EndLevel.h"
#include "Aladdin.h"
#include "CameraView.h"
#include "ScoreBoard.h"
#include "box2d/box2d.h"

//----------------------------------class definition----------------------------------
class World
{
public:
	World();
	~World();

	void printWorld(sf::RenderWindow&) const;
	void updateObject(const sf::Time&);
	void stepBox2dWorld();
	void resetWorld();

	sf::FloatRect getPlayButtonGloBu() const;
	void setCameraView(sf::RenderWindow&) const;
	bool getIsGameFinished() const;
	int getPlayerScore() const;
	sf::Vector2f getAladdinPos() const;

private:
	void addWeapon(Media::Objects, sf::Vector2f, Direction);
	void readLevel();
	void handleCollision();
	void destroyAllBodies();
	void updateLevelBackground();
	void resetObjects();
	void removeBodyIfDisposed(GameObject*);
	void checkGameStatus();

	int  m_level;				  // the current level number
	bool m_gameFinished = false,  // true if the game is over
		m_isNewLevel = false;    // true if we started a new level
	Aladdin m_aladdin;              // holds the main player
	CameraView m_worldView;         // current view out of the entire screen
	ScoreBoard m_scoreBoard;        // the score board of the current data of the game to be shown on screen
	std::ifstream& m_levelFile;     // the file that holds the data of the levels

	b2World m_box2dWorld;			// the box2D world to be attached to the game

	using uniqueMovablesVec = std::vector<std::unique_ptr<Movables>>;
	using uniqueUnmovableVec = std::vector<std::unique_ptr<Unmovables>>;
	using uniqueWeaponsVec = std::vector<std::unique_ptr<Weapon>>;
	uniqueMovablesVec	m_movables;
	uniqueUnmovableVec	m_unmovables;
	uniqueWeaponsVec	m_weapons;

	EndLevel m_endLevel;

	struct levelBackground
	{
		sf::Sprite _bg,
				   _pillars,
			       _sky;
	};
	levelBackground levelBg;

};