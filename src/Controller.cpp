//----------------------------------include section-----------------------------------
#include "Controller.h"
#include "Aladdin.h"
#include "macros.h"
#include <SFML/Graphics.hpp>
#include <iostream>

//----------------------------------class implementation------------------------------

void Controller::run()
{
	//iterate until the user decides to exit the game
	while (true)
	{
		m_menu.openMenu();
		Media::instance().playMusic(Media::s_bgMusic);

		m_world.resetWorld();
		sf::RenderWindow window(sf::VideoMode(1200, 800),
			"Aladdin", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

		sf::Clock clock;

		while (window.isOpen())
		{
			window.clear();
			m_world.setCameraView(window);  //show the view in the size of the window
			m_world.stepBox2dWorld();		//update box2d objects

			// Handle events
			sf::Event event;
			while (window.pollEvent(event))
			{
				//Window closed or escape key pressed: exit
				switch (event.type)
				{
				case sf::Event::Closed: window.close(); return;

				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						return;
					}
					break;
				case sf::Event::MouseButtonReleased:
				{
					auto clickLocation = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
					//the user decided to mute/unmute the background music
					if (m_world.getPlayButtonGloBu().contains(clickLocation))
					{
						Media::instance().playSound(Media::s_click);
						Media::instance().switchBgMusic();
					}
				}
				break;
				}
			}
			sf::Time deltaTime = clock.restart();
			m_world.updateObject(deltaTime);
			if (m_world.getIsGameFinished())
			{
				m_menu.addScore(m_world.getPlayerScore()); //add the player to high score board
				window.close();
			}
			m_world.printWorld(window);
			window.display();
		}
	}

}
