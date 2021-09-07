//----------------------------------include section-------------------------------------
#include "ScoreBoard.h"
#include <string.h>

//----------------------------------include section-------------------------------------

const float MUTE_SCALE = 0.3;
const sf::Vector2f APPLE_WE_SCALE = { 2,2 };
const std::string SPACE = " ", P_SCORE = "Score ";
//----------------------------------class implementation------------------------------

ScoreBoard::ScoreBoard()
{
	configureTextMembers();
	configureSpriteMembers();
}

//----------------------------------functions section---------------------------------

void ScoreBoard::drawScoreBoard(sf::RenderWindow& window) const
{
	window.draw(m_textScore);
	window.draw(m_textLives);
	window.draw(m_textWeapons);
	window.draw(m_heartLife);
	window.draw(m_appleWeapon);
	window.draw(m_play);
	if(!Media::instance().isBgMusicPlaying())
		window.draw(m_mute);
}

void ScoreBoard::configureTextMembers()
{
	Media& ins = Media::instance();

	ins.configureText(m_textScore, sf::Color::Black, sf::Color::Yellow);
	ins.configureText(m_textLives, sf::Color::Black, sf::Color(109, 148, 247));
	ins.configureText(m_textWeapons, sf::Color::Black, sf::Color(158, 29, 57));
}

void ScoreBoard::configureSpriteMembers()
{
	m_heartLife.setTexture(Media::instance().getTexture(Media::t_items));
	m_heartLife.setTextureRect(sf::IntRect(343, 171, 19, 24));
	m_appleWeapon.setTexture(Media::instance().getTexture(Media::t_items));
	m_appleWeapon.setTextureRect(sf::IntRect(341, 17, 11, 12));
	m_appleWeapon.setScale(APPLE_WE_SCALE);

	m_play.setTexture(Media::instance().getTexture(Media::t_play));
	m_mute.setTexture(Media::instance().getTexture(Media::t_mute));
	m_mute.setScale(MUTE_SCALE, MUTE_SCALE);

}

void ScoreBoard::updatePosition(sf::View view)
{
	sf::Vector2f size = view.getSize();
	sf::Vector2f center = view.getCenter();
	
	//specific calculation according to the camera view
	m_textScore.setPosition(center.x - (size.x / 2) + 20.f, center.y - (size.y / 2) + 10.f);
	m_appleWeapon.setPosition(center.x + (size.x / 5), center.y - (size.y / 2) + 25.f);
	m_textWeapons.setPosition(m_appleWeapon.getPosition().x + m_appleWeapon.getGlobalBounds().width, 
								m_appleWeapon.getPosition().y - 15.f);
	m_heartLife.setPosition(center.x + (size.x / 3) + 10.f, center.y - (size.y / 2) + 30.f);
	m_textLives.setPosition(m_heartLife.getPosition().x + m_heartLife.getGlobalBounds().width,
		m_heartLife.getPosition().y - 20.f);
	
	m_play.setPosition(m_textLives.getPosition().x + 55, m_textLives.getPosition().y + 15);
	m_mute.setPosition(m_play.getPosition().x, m_play.getPosition().y +5);
}

void ScoreBoard::updateText(const PlayerData& data)
{
	m_textScore.setString(P_SCORE + std::to_string(data._score));
	m_textLives.setString(SPACE + std::to_string(data._lives));
	m_textWeapons.setString(SPACE + std::to_string(data._weapons));
}

void ScoreBoard::updateScoreBoard(sf::View view, const PlayerData& data)
{
	updatePosition(view);
	updateText(data);
}

sf::FloatRect ScoreBoard::getPlayButtonGloBu() const
{
	return m_play.getGlobalBounds();
}
