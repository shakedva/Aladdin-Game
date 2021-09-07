//----------------------------------include section-----------------------------------
#pragma once
#include "macros.h"
#include <SFML/Graphics.hpp>
#include "Media.h"

//----------------------------------class definition----------------------------------
class ScoreBoard 
{
public:
	ScoreBoard();
	~ScoreBoard() = default;
	void drawScoreBoard(sf::RenderWindow& window) const;
	void updateScoreBoard(sf::View view, const PlayerData& data);
	sf::FloatRect getPlayButtonGloBu() const;

private:
	void configureTextMembers();
	void configureSpriteMembers();
	void updatePosition(sf::View view);
	void updateText(const PlayerData& data);

	sf::Text	m_textScore,    // the text that represents the score on screen
				m_textLives,	// the text that represents the lives on screen
				m_textWeapons;  // the text that represents the num of weapons on screen
	sf::Sprite  m_appleWeapon,  // the appleWeapon sprite to be displayed on screen
				m_heartLife;	// the hearts sprite to be displayed on screen
	sf::Sprite m_play, m_mute; // represent the play and mute buttons

};