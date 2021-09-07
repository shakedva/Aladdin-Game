//----------------------------------include section---------------------------------
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "AnimationData.h"

//---------------------------------class implementation-----------------------------
//Media is a Singelton pattern
class Media
{
public:
	enum Objects
	{
		Aladdin,
		Apple,
		LivesGift,
		Ruby,
		KnifeWeapon,
		NearEnemy,
		Genie,
		Background,
		Pillars,
		Sky,
		AppleWeapon,
		Wall,
		Floor,
		Rope,
		Pole,
		Stair,
		DistantEnemy,
		JugglingEnemy,
		EndLevel,
		Max
	};

	enum Textures
	{
		t_aladdin,
		t_items,
		t_nonStaticEnemy,
		t_staticEnemy,
		t_genie,
		t_background,
		t_pillars,
		t_sky,
		t_menu,
		t_aladdinLogo,
		t_nameTag,
		t_sultansDungeon,
		t_sultansPillars,
		t_help,
		t_play,
		t_mute,
		t_endLevel,
		t_end
	};

	enum Fonts
	{
		f_aladdin,
		f_scores,
		f_end
	};

	enum Sounds
	{
		s_click,
		s_collectRuby,
		s_collectGenie,
		s_extraHealth,
		s_aliSword,
		s_knifeWeapon,
		s_levelComplete,
		s_collectApp,
		s_enemyDis,
		s_throwApp,
		s_aliRestart,
		s_jump,
		s_bgMusic,
		s_menuMusic,
		s_end
	};

	~Media();
	static Media& instance();

	void playMusic(int);
	bool isBgMusicPlaying() const;
	void switchBgMusic();
	void playSound(int);
	const AnimationData& animationData(int object) { return m_data[object]; }

	sf::Image& getCursorImg();
	sf::Font& getFont(int);
	std::ifstream& getObjectsPosFile();
	std::fstream& getHighScoresFile();
	const sf::Texture& getTexture(int);
	void configureText(sf::Text&, sf::Color, sf::Color) const;

	sf::Text setText(unsigned int, sf::Vector2f, sf::Color, std::string, int, int) const;
	sf::RectangleShape createRec(sf::Vector2f, sf::Vector2f);
private:
	Media(); // private constructor
	void loadTextures();
	void loadAudio();
	void loadFile();
	void loadFont();
	void loadAnimationData();

	std::ifstream					m_animationFile, // file that holds all the animations
		m_objectsFile;	 // file that holds all the objects locations
	std::fstream					m_highScoresFile;// file that holds all high scores
	std::vector <sf::Font>			m_font;			 // saves the font that will be shown in the game
	std::vector <sf::Texture>		m_textures;		 // vector for all textures
	std::vector<AnimationData>		m_data;			 // the animation data for each object
	sf::Image m_cursorImg;							 // saves the image to be shown as the cursor in the game

	sf::Music						m_bgMusic,		 // saves the background music
									m_menuMusic;
	std::vector <sf::Sound>			m_sounds;		 // saves all sounds
	std::vector <sf::SoundBuffer>	m_soundBuffer;	 // saves all sound buffers
};