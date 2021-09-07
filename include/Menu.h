//----------------------------------include section---------------------------------

#pragma once
#include <SFML/Graphics.hpp>
#include "HighScoreBoard.h"
#include "Media.h"

//----------------------------------include section---------------------------------
const std::string NEW_GAME = "New Game", HIGH_SCORE = "High Scores", HELP = "Help", EXIT = "Exit Game",
				  BACK = "back", SAVE = "Save", NAME = "Name", SCOR = "Score", ENTER = "Enter Name", ENTER_Q = "Please enter your name:",
					INFO = "Created by:\nShaked Vaknin\nNoy Tal\nSalomon Demma\nRina Shushan";
const int BACKSPACE = 8;
				
//----------------------------------struct definition---------------------------------

// Struct to represent the menu's features
struct MenuFeature
{
	sf::Sprite _bg = sf::Sprite(Media::instance().getTexture(Media::t_menu));
	sf::Sprite _logo = sf::Sprite(Media::instance().getTexture(Media::t_aladdinLogo));
	sf::Sprite _help = sf::Sprite(Media::instance().getTexture(Media::t_help));
	sf::Sprite _nameTag = sf::Sprite(Media::instance().getTexture(Media::t_nameTag));
	sf::Cursor _cursor;
	std::map<std::string, sf::Text> _buttons; // Map to represent possible buttons in the game

	void updateMenuFeatures()
	{
		Media& ins = Media::instance();

		_bg.setPosition(0.f, 0.f);
		_bg.setScale(1700.f / _bg.getLocalBounds().width, 900.f / _bg.getLocalBounds().height);
		_logo.setPosition(320.f, 0.f);
		_help.setScale({ 1.5f, 1.5f });
		_help.setPosition(160.f, 150.f);
		_nameTag.setPosition(300.f, 400.f);
		_nameTag.setScale({1.5, 1});

		auto gold = sf::Color(229, 153, 64);
		_buttons[NEW_GAME] = ins.setText(75, { 430, 300 }, gold, NEW_GAME, 4, Media::f_aladdin);
		_buttons[HIGH_SCORE] = ins.setText(75, { 430, 400 }, gold, HIGH_SCORE, 4, Media::f_aladdin);
		_buttons[HELP] = ins.setText(75, { 550, 500 }, gold, HELP, 4, Media::f_aladdin);
		_buttons[EXIT] = ins.setText(75, { 460, 600 }, gold, EXIT, 4, Media::f_aladdin);
		_buttons[BACK] = ins.setText(50, { 50, 700 }, gold, BACK, 3, Media::f_scores);
		_buttons[SAVE] = ins.setText(50, { 880, 400 }, gold, SAVE, 3, Media::f_scores);
		_buttons[NAME] = ins.setText(70, { 300, 150 }, sf::Color::Black, NAME, 0, Media::f_aladdin);
		_buttons[SCOR] = ins.setText(70, { 750, 150 }, sf::Color::Black, SCOR, 0, Media::f_aladdin);
		_buttons[ENTER] = ins.setText(50, { 300, 320 }, sf::Color::Black, ENTER_Q, 0, Media::f_scores);
		_buttons[INFO] = ins.setText(30, { 1450, 40 }, gold, INFO, 1, Media::f_scores);
	}
};

//----------------------------------class definition---------------------------------
class Menu
{
public:
	Menu();
	~Menu() = default;

	void openMenu();
	void addScore(int);
	
private:
	void openHighScoresBoard(sf::RenderWindow&);
	void openHelpBoard(sf::RenderWindow&);
	void openReceivePlayerName(sf::RenderWindow& ,bool&);
	void drawMenuButtons(sf::RenderWindow&);
	void drawHighScoreButtons(sf::RenderWindow&, sf::RectangleShape);
	void drawHelpButtons(sf::RenderWindow&, sf::RectangleShape);
	void drawPlayerNameButtons(sf::RenderWindow&, sf::RectangleShape, sf::Text&);
	void setOutlineWhenCursorMoved(sf::Text&, sf::Color) const;
	void setOutline(sf::Text&, sf::Vector2f) const;
	
	MenuFeature m_feature;
	HighScoreBoard m_highScoresBoard; // the high score table to be shown on screen
	std::string m_playersName = "";	  // the current player's name to be shown in the high score table
};