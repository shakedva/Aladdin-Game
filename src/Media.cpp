//----------------------------------include section---------------------------------
#include "Media.h"
#include "macros.h"
#include <stdexcept>

//----------------------------------const section---------------------------------
const int VOL = 15, PLAY = 2;
const std::string   TXTURE_ERR = "Failed to load texture\nPress any key to continue...",
					WORLD_OBJ_ERR = "File worldObjectsPos.txt could not open\nPress any key to continue...",
					HIGH_SCORE_ERR = "File high_score_table.txt could not open\nPress any key to continue...",
					FONT_ERR = "Failed to load font\nPress any key to continue...";

//----------------------------------namespace section---------------------------------
namespace
{
	AnimationData readAnimation(std::string fileName)
		// get a file name the contain the coordinate of the animmation;
	{
		auto file = std::ifstream(fileName);
		int numAnime,				// num of the animation in the file
			numOfFrame;				// coordinate for single frame 

		file >> numAnime;
		auto animations = AnimationData{};

		for (int animationKey = 0; animationKey < numAnime; animationKey++)
		{
			file >> numOfFrame;
			sf::Vector2i size,			// x for width, y for heigh
				currentStart;			// coordinate in the pictute
			sf::Vector2f originStart;

			for (int frameIndex = 0; frameIndex < numOfFrame; frameIndex++)
			{
				file >> currentStart.x >> currentStart.y >> size.x >> size.y
					>> originStart.x >> originStart.y;

				animations.m_animeMap[animationKey].emplace_back(currentStart, size);
				animations.m_originPoint[animationKey].emplace_back(originStart);
			}
		}
		return animations;
	}
}

//----------------------------------class implementation------------------------------

// constructor that opens all the files needed for the game
Media::Media()
{
	loadFont();
	loadTextures();
	loadAudio();
	loadFile();
	loadAnimationData();
}


// destructor that closes all the files 
Media::~Media()
{
	m_animationFile.close();
	m_objectsFile.close();
	m_highScoresFile.close();
}
//----------------------------------functions section---------------------------------

// load all the textures for the game and push it into m_textures
void Media::loadTextures()
{
	m_cursorImg.loadFromFile("lamp.png");
	// push into Texture vector 
	m_textures.resize(t_end);
	if (!m_textures[t_aladdin].loadFromFile("Aladdin_trans.png") ||
		!m_textures[t_items].loadFromFile("Items_trans.png") ||
		!m_textures[t_nonStaticEnemy].loadFromFile("Enemies_1_trans.png") ||
		!m_textures[t_staticEnemy].loadFromFile("Enemies_Sprite_2_trans.png") ||
		!m_textures[t_background].loadFromFile("AgrabahMarket - Copy.png") ||
		!m_textures[t_pillars].loadFromFile("pillars.png") ||
		!m_textures[t_sky].loadFromFile("111.jpg") ||
		!m_textures[t_menu].loadFromFile("menu.jpg") ||
		!m_textures[t_aladdinLogo].loadFromFile("aladdin_logo.png") ||
		!m_textures[t_sultansDungeon].loadFromFile("SultansDungeon.png") ||
		!m_textures[t_sultansPillars].loadFromFile("Dungeon_Pillars.png") ||
		!m_textures[t_help].loadFromFile("help.png") ||
		!m_textures[t_endLevel].loadFromFile("endLevel.png"))
		throw std::runtime_error(TXTURE_ERR);

	// if failed to load any of the following textures, the game can continue without it
	m_textures[t_nameTag].loadFromFile("nameTag.png");
	m_textures[t_play].loadFromFile("play.png");
	m_textures[t_mute].loadFromFile("mute.png");
	m_textures[t_aladdin].setSmooth(true);
	m_textures[t_items].setSmooth(true);
	m_textures[t_nonStaticEnemy].setSmooth(true);
	m_textures[t_staticEnemy].setSmooth(true);
	m_textures[t_background].setSmooth(true);
	m_textures[t_pillars].setSmooth(true);
	m_textures[t_sky].setSmooth(true);
	m_textures[t_menu].setSmooth(true);
	m_textures[t_sultansDungeon].setSmooth(true);
	m_textures[t_sultansPillars].setSmooth(true);
	m_textures[t_help].setSmooth(true);
	m_textures[t_endLevel].setSmooth(true);
}

//load all the audio for the game and saves it the the class's members
void Media::loadAudio()
{
	// if one of the sounds could not open, do not throw an exceptions
	// the game can continue without it
	if (m_bgMusic.openFromFile("resources/sound-resources/prince_ali.wav"))
	{
		m_bgMusic.setVolume(VOL);
	}
	if (m_menuMusic.openFromFile("resources/sound-resources/Menu.wav"))
	{
		m_menuMusic.setVolume(VOL);
		m_menuMusic.setLoop(true);
		m_menuMusic.play();
	}

	sf::SoundBuffer clickBuf, collectRubyBuf, collectGenieBuf, extraHealthBuf,
		aliSwordBuffer, knifeWeaponBuf, levelCompleteBuf, collectAppBuf, enemyDisBuf,
		throwAppleBuf, aliRestartBuf, jumpBuf;

	clickBuf.loadFromFile("resources/sound-resources/click.wav");
	collectRubyBuf.loadFromFile("resources/sound-resources/collectRuby.wav");
	collectGenieBuf.loadFromFile("resources/sound-resources/collectGenie.wav");
	extraHealthBuf.loadFromFile("resources/sound-resources/Extra Health.wav");
	aliSwordBuffer.loadFromFile("resources/sound-resources/aliSword.wav");
	knifeWeaponBuf.loadFromFile("resources/sound-resources/knifeWeapon.wav");
	levelCompleteBuf.loadFromFile("resources/sound-resources/level_complete.wav");
	collectAppBuf.loadFromFile("resources/sound-resources/collectApple.wav");
	enemyDisBuf.loadFromFile("resources/sound-resources/enemyDis.wav");
	throwAppleBuf.loadFromFile("resources/sound-resources/throwApple.wav");
	aliRestartBuf.loadFromFile("resources/sound-resources/aliRestart.wav");
	jumpBuf.loadFromFile("resources/sound-resources/jump.wav");

	m_soundBuffer.push_back(clickBuf);
	m_soundBuffer.push_back(collectRubyBuf);
	m_soundBuffer.push_back(collectGenieBuf);
	m_soundBuffer.push_back(extraHealthBuf);
	m_soundBuffer.push_back(aliSwordBuffer);
	m_soundBuffer.push_back(knifeWeaponBuf);
	m_soundBuffer.push_back(levelCompleteBuf);
	m_soundBuffer.push_back(collectAppBuf);
	m_soundBuffer.push_back(enemyDisBuf);
	m_soundBuffer.push_back(throwAppleBuf);
	m_soundBuffer.push_back(aliRestartBuf);
	m_soundBuffer.push_back(jumpBuf);

	for (int i = 0; i < m_soundBuffer.size(); i++)
		m_sounds.push_back(sf::Sound(m_soundBuffer[i]));
}

void Media::loadFile()
{
	std::string path = "worldObjectsPos.txt";
	m_objectsFile.open(path);
	if (!m_objectsFile.is_open())
	{
		throw std::runtime_error(WORLD_OBJ_ERR);
	}
	path = "high_score_table.txt";
	m_highScoresFile.open(path);
	if (!m_highScoresFile.is_open())
	{
		throw std::runtime_error(HIGH_SCORE_ERR);
	}

}

void Media::loadFont()
{
	m_font.resize(f_end);
	if (!m_font[f_aladdin].loadFromFile("aladdin.ttf") || !m_font[f_scores].loadFromFile("Arabian Knight.otf"))
		throw std::runtime_error(FONT_ERR);

}

void Media::loadAnimationData()
{
	m_data.resize(Media::Max);
	m_data[Media::Aladdin] = readAnimation("Aladdin.txt");
	m_data[Media::NearEnemy] = readAnimation("NearEnemy.txt");
	m_data[Media::DistantEnemy] = readAnimation("DistantEnemy.txt");
	m_data[Media::JugglingEnemy] = readAnimation("StaticEnemy.txt");
	m_data[Media::AppleWeapon] = readAnimation("AppleWeapon.txt");
	m_data[Media::KnifeWeapon] = readAnimation("KnifeWeapon.txt");
	m_data[Media::Ruby] = readAnimation("Ruby.txt");
	m_data[Media::LivesGift] = readAnimation("LivesGift.txt");
	m_data[Media::Genie] = readAnimation("Genie.txt");
	m_data[Media::Apple] = readAnimation("AppleItem.txt");
	m_data[Media::EndLevel] = readAnimation("EndGame.txt");
}

Media& Media::instance()
{
	// the object inst is created once and can be reached everywhere in the game
	try
	{
		static Media inst;
		return inst;
	}
	catch (std::exception& e)
	{
		sf::Font font;
		font.loadFromFile("Arabian Knight.otf"); // there is no default font in sf::Text

		sf::RenderWindow window(sf::VideoMode(1200, 500), "Exception", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		sf::Text exc;
		exc.setFont(font);
		exc.setCharacterSize(50);
		exc.setPosition({ 20,20 });
		exc.setFillColor(sf::Color::White);
		exc.setString(e.what());
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed: window.close(); exit(EXIT_FAILURE); break;

				case sf::Event::KeyPressed: window.close(); exit(EXIT_FAILURE); break;

				case sf::Event::MouseButtonReleased: window.close(); exit(EXIT_FAILURE); break;
				}
			}
			window.clear();
			window.draw(exc);
			window.display();
		}
	}
}

void Media::switchBgMusic()
{
	isBgMusicPlaying() ? m_bgMusic.pause() : m_bgMusic.play();	
}
void Media::playMusic(int music)
{
	switch (music)
	{
	case s_bgMusic: m_bgMusic.play();
		m_bgMusic.setLoop(true);
		m_menuMusic.pause();
		break;
	case s_menuMusic: m_menuMusic.play(); 
		m_menuMusic.setLoop(true);
		m_bgMusic.pause();
		break;
	}
}

bool Media::isBgMusicPlaying() const
{
	return m_bgMusic.getStatus() == PLAY;
}

// playes the wanted sound effect 
void Media::playSound(int index)
{
	m_sounds[index].play();
}

//----------------------------------set functions section---------------------------------

//reveive a text and set its style
void Media::configureText(sf::Text& txt, sf::Color fillColor, sf::Color outlineColor) const
{
	txt.setFont(Media::instance().getFont(Media::f_aladdin));
	txt.setColor(fillColor);
	txt.setStyle(sf::Text::Bold);
	txt.setOutlineThickness(2);
	txt.setOutlineColor(outlineColor);
	txt.setCharacterSize(40);

}

//returns a text, and updates its characters and wanted features
sf::Text Media::setText(unsigned int size, sf::Vector2f pos, sf::Color fillColor, std::string str, int outlineThickness, int typeFont) const
{
	sf::Text txt;
	txt.setFont(Media::instance().getFont(typeFont));
	txt.setCharacterSize(size);
	txt.setPosition(pos);
	txt.setFillColor(fillColor);
	txt.setStyle(sf::Text::Bold);
	txt.setOutlineThickness(outlineThickness);
	txt.setString(str);
	return txt;
}

//creating a half transparent rectangle
sf::RectangleShape Media::createRec(sf::Vector2f pos, sf::Vector2f size)
{
	sf::RectangleShape rec;
	rec.setPosition(pos);
	rec.setSize(size);
	rec.setFillColor(sf::Color{ 255, 255, 255, 70 });
	return rec;
}

//----------------------------------get functions section---------------------------------

// returns the wanted texture
const sf::Texture& Media::getTexture(int index) { return m_textures[index]; }

// returns the wanted font
sf::Font& Media::getFont(int index) { return m_font[index]; }

std::ifstream& Media::getObjectsPosFile() { return m_objectsFile; }

std::fstream& Media::getHighScoresFile() { return m_highScoresFile; }

sf::Image& Media::getCursorImg() { return m_cursorImg; }