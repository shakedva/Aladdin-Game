//----------------------------------include section-----------------------------------
#pragma once
#include "World.h"
#include "Menu.h"

//----------------------------------class definition----------------------------------
class Controller
{
public:
	Controller() = default;
	~Controller() = default;
	void run();

private:

	sf::Time m_time; // the current time of the game
	World m_world;   // attaches the world of the game to the display on the screen
	Menu m_menu;	// the menu of the game

};