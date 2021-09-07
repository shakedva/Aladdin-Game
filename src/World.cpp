//----------------------------------include section-----------------------------------
#include <experimental/vector>
#include <iostream>
#include <typeinfo>

#include "World.h"
#include "Media.h"
#include "Aladdin.h"
#include "Apple.h"
#include "Ruby.h"
#include "Genie.h"
#include "LivesGift.h"
#include "Floor.h"
#include "Weapon.h"
#include "Wall.h"
#include "Rope.h"
#include "Enemy.h"
#include "CollisionHandling.h"
#include "AppleWeapon.h"
#include "NearEnemy.h"
#include "DistantEnemy.h"
#include "JugglingEnemy.h"
#include "KnifeWeapon.h"
//----------------------------------const section-------------------------------------
const int32 m_velocityIterations = 6, // velocity of the world
			m_positionIterations = 2; // position iterations of the world
const float m_timeStep = 1.0f / 60.0,// timeStep of the world
			s_lvl1 = 35.f, s_lvl2 = 30.f;
const b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
const sf::Vector2f initVec = sf::Vector2f(0.f, 0.f);
const int OFFSET = 37;

//----------------------------------class implementation------------------------------

World::World() :
	m_level(INIT),
	m_box2dWorld(gravity),
	m_aladdin(initVec), m_endLevel(initVec),
	m_levelFile(Media::instance().getObjectsPosFile()) {}

World::~World()
{
	m_endLevel.destroyBody();
	m_aladdin.destroyBody();
	destroyAllBodies();
}

//----------------------------------functions section---------------------------------

void World::readLevel()
{	
	if (m_levelFile.eof()) //read the game until reaching the end of the file
	{
		m_gameFinished = true;
		return;
	}
	m_level++;
	updateLevelBackground();
	resetObjects();
	int numOfObjects;
	m_levelFile >> numOfObjects; //read how many objects there are in the game

	int objectId;
	sf::Vector2f pos,
				limit;
	for (int i = 0; i < numOfObjects; i++)
	{
		m_levelFile >> objectId >> pos.x >> pos.y;
		switch (objectId)
		{
		case Media::Aladdin:
		{
			if (m_aladdin.getBody() == nullptr)
				m_aladdin.createBody(&m_box2dWorld, pos);
			m_aladdin.setDefaultPosition(pos);
			m_aladdin.setAladdinToDefaultPosition();
			break;
		}
		case Media::NearEnemy:		//Muscle
		{
			m_levelFile >> limit.x >> limit.y;
			m_movables.emplace_back(std::make_unique<NearEnemy>(pos, m_aladdin, limit));
			m_movables[m_movables.size() - 1]->createBody(&m_box2dWorld, pos);
			break;
		}
		case Media::DistantEnemy:	//Fat
		{
			m_levelFile >> limit.x >> limit.y;
			m_movables.emplace_back(std::make_unique<DistantEnemy>(pos, m_aladdin, limit));
			m_movables[m_movables.size() - 1]->createBody(&m_box2dWorld, pos);
			break;
		}
		case Media::JugglingEnemy:
		{
			m_movables.emplace_back(std::make_unique<JugglingEnemy>(pos, m_aladdin));
			m_movables[m_movables.size() - 1]->createBody(&m_box2dWorld, pos);
			break;
		}

		case Media::Apple:
			m_unmovables.emplace_back(std::make_unique<Apple>(pos)); break;
		case Media::Ruby:
			m_unmovables.emplace_back(std::make_unique<Ruby>(pos)); break;
		case Media::Genie:
			m_unmovables.emplace_back(std::make_unique<Genie>(pos)); break;
		case Media::LivesGift:
			m_unmovables.emplace_back(std::make_unique<LivesGift>(pos)); break;
		case Media::EndLevel:
			m_endLevel.setPosition(pos); break;

		case Media::Floor: case Media::Wall: case Media::Rope:
		{
			sf::Vector2f size;
			m_levelFile >> size.x >> size.y; //read the object's size as well

			if (objectId == Media::Rope)
			{
				m_unmovables.emplace_back(std::make_unique<Rope>(pos, size));
				break;
			}
			switch (objectId)
			{
			case Media::Floor:
				m_unmovables.emplace_back(std::make_unique<Floor>(pos, size));
				m_unmovables[m_unmovables.size() - 1]->createBody(&m_box2dWorld, pos, size);
				break;
			case Media::Wall:
				m_unmovables.emplace_back(std::make_unique<Wall>(pos, size));
				m_unmovables[m_unmovables.size() - 1]->createBody(&m_box2dWorld, pos, size);
				break;
			}
			break;
		}
		}
	}
}

//change background images according to the level, and set aladdins speed
void World::updateLevelBackground()
{
	switch (m_level)
	{
	case LVL1:
		levelBg._bg = sf::Sprite(Media::instance().getTexture(Media::t_background));
		levelBg._pillars = sf::Sprite(Media::instance().getTexture(Media::t_pillars));
		levelBg._sky = sf::Sprite(Media::instance().getTexture(Media::t_sky));
		m_aladdin.setSpeed(s_lvl1);
		break;
	case LVL2:
		levelBg._bg = sf::Sprite(Media::instance().getTexture(Media::t_sultansDungeon));
		levelBg._pillars = sf::Sprite(Media::instance().getTexture(Media::t_sultansPillars));
		m_aladdin.setSpeed(s_lvl2);
		break;
	}
}

void World::stepBox2dWorld()
{
	m_box2dWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
}

//print all the game objects and background to the screen
void World::printWorld(sf::RenderWindow& window) const
{
	window.draw(levelBg._sky);
	window.draw(levelBg._bg);
	for (auto& unmovable : m_unmovables)
		unmovable->draw(window);

	for (auto& character : m_movables)
		character->draw(window);

	for (auto& weapon : m_weapons)
		weapon->draw(window);

	m_aladdin.draw(window);
	m_endLevel.draw(window);
	window.draw(levelBg._pillars);
	m_scoreBoard.drawScoreBoard(window);
}

//update all objects' actions and locations
void World::updateObject(const sf::Time& deltaTime)
{
	m_aladdin.updateAction(deltaTime);
	m_worldView.updateView(m_aladdin.getPosition(), m_level);
	checkGameStatus();

	if (m_aladdin.isThrowingWeapon())
	{
		addWeapon(Media::Objects::AppleWeapon, m_aladdin.getPosition(), m_aladdin.getDirection());
		m_aladdin.setThrowingWeapon(false);
	}

	for (auto& movable : m_movables)
	{
		movable->updateAction(deltaTime);
		if (movable->isThrowingWeapon())
		{
			addWeapon(Media::Objects::KnifeWeapon, movable->getPosition(), movable->getDirection());
			movable->setThrowingWeapon(false);
		}
	}

	for (auto& weapon : m_weapons)
		weapon->updateAction(deltaTime);

	for (auto& unmovable : m_unmovables)
		unmovable->animateUnmovable(deltaTime);
	//m_endLevel.animateUnmovable(deltaTime);

	m_scoreBoard.updateScoreBoard(m_worldView.getView(), m_aladdin.getAladdinData());
	handleCollision();
	stepBox2dWorld();
}

//check all possible collisions
void World::handleCollision()
{
	for (auto& unmovable : m_unmovables)
	{
		if (m_aladdin.checkCollision(unmovable->getGlobalBounds()))
			processCollision(m_aladdin, *unmovable);
		for (auto& movable : m_movables)
			if (movable->checkCollision(unmovable->getGlobalBounds()))
				processCollision(*movable, *unmovable);

		for (auto& weapon : m_weapons)
		{
			if (weapon->checkCollision(unmovable->getGlobalBounds()))
			{
				processCollision(*weapon, *unmovable);
				removeBodyIfDisposed(weapon.get());
			}
		}
	}
	for (auto& movable : m_movables)
	{
		if (m_aladdin.checkCollision(movable->getGlobalBounds()))
		{
			processCollision(m_aladdin, *movable);
			removeBodyIfDisposed(movable.get());
		}
	}
	for (auto& movable : m_movables)
	{
		for (auto& weapon : m_weapons)
		{
			if (weapon->checkCollision(movable->getGlobalBounds()))
			{
				processCollision(*weapon, *movable);
				removeBodyIfDisposed(movable.get());
				removeBodyIfDisposed(weapon.get());
			}
		}
	}
	for (auto& weapon : m_weapons)
	{
		for (auto& weapon2 : m_weapons)
		{
			if (weapon->checkCollision(weapon2->getGlobalBounds()))
			{
				processCollision(*weapon, *weapon2);
				removeBodyIfDisposed(weapon.get());
				removeBodyIfDisposed(weapon2.get());
			}
		}
		if (m_aladdin.checkCollision(weapon->getGlobalBounds()))
		{
			processCollision(m_aladdin, *weapon);
			removeBodyIfDisposed(weapon.get());
		}
	}
	//erase from the objects vectors the objects that are disposed
	std::experimental::erase_if(m_movables, [](const auto& item) { return item->isDisposed(); });
	std::experimental::erase_if(m_unmovables, [](const auto& item) { return item->isDisposed(); });
	std::experimental::erase_if(m_weapons, [](const auto& item) { return item->isDisposed(); });

}

//delete all box2d bodies when ending a level
void World::destroyAllBodies()
{
	if (m_box2dWorld.GetBodyCount() > INIT)
	{
		for (auto& movable : m_movables)
			if (movable->getBody() != nullptr)
				m_box2dWorld.DestroyBody(movable->getBody());

		for (auto& unmovable : m_unmovables)
			if (unmovable->getBody() != nullptr)
				m_box2dWorld.DestroyBody(unmovable->getBody());

		for (auto& weapon : m_weapons)
			if (weapon->getBody() != nullptr)
				m_box2dWorld.DestroyBody(weapon->getBody());
	}
}

//reset all objects vectors and bodies
void World::resetObjects()
{
	destroyAllBodies(); // delete all m_box2d bodies
	m_unmovables.clear();
	m_movables.clear();
	m_weapons.clear();
}

//if an object was disposed, remove his box2d body
void World::removeBodyIfDisposed(GameObject* gameObject)
{
	if (gameObject->getBody() && gameObject->isDisposed())
	{
		m_box2dWorld.DestroyBody(gameObject->getBody());
		gameObject->destroyBody();
	}
}

//check the current game data to know how to continue the game
void World::checkGameStatus()
{
	if (m_aladdin.getAladdinData()._lives == INIT)
		m_gameFinished = true;

	if (m_aladdin.checkCollision(m_endLevel.getGlobalBounds()))
		processCollision(m_aladdin, m_endLevel);

	if (m_endLevel.isDisposed())
	{
		m_endLevel.setDisposed(false);
		readLevel();
		return;
	}
}

void World::addWeapon(Media::Objects obj, sf::Vector2f pos, Direction dir)
{
	pos.y -= OFFSET;
	switch (obj)
	{
	case Media::KnifeWeapon:
		m_weapons.emplace_back(std::make_unique<KnifeWeapon>(pos, dir));
		Media::instance().playSound(Media::s_knifeWeapon);
		break;
	case Media::AppleWeapon:
		m_weapons.emplace_back(std::make_unique<AppleWeapon>(pos, dir));
		Media::instance().playSound(Media::s_throwApp);
		break;
	default:
		return;
	}
	m_weapons[m_weapons.size() - 1]->createBody(&m_box2dWorld, pos);
}


//reset all the game data bedore starting a new level/game
void World::resetWorld()
{
	if (m_gameFinished)
	{
		m_aladdin.resetAladdinData();
		m_levelFile.clear();
		m_levelFile.seekg(INIT);
		m_level = INIT;
		m_endLevel.setPosition(initVec);
		m_gameFinished = false;
	}
	resetObjects();
	readLevel();
}

//----------------------------------set functions section---------------------------------

void World::setCameraView(sf::RenderWindow& window) const { m_worldView.setView(window); }

//----------------------------------get functions section---------------------------------

sf::FloatRect World::getPlayButtonGloBu() const { return m_scoreBoard.getPlayButtonGloBu(); }

sf::Vector2f World::getAladdinPos() const { return m_aladdin.getAladdinPos(); }

bool World::getIsGameFinished() const { return m_gameFinished; }

int World::getPlayerScore() const { return m_aladdin.getAladdinData()._score; }