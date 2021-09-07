//----------------------------------include section-----------------------------------
#include "CameraView.h"
#include "macros.h"
#include <iostream>

//----------------------------------const section-----------------------------------
const float CAMERA_LEFT = 0.f,
CAMERA_TOP = 300.f,
CAMERA_WIDTH = 700.f,
CAMERA_HEIGHT = 380.f;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
CameraView::CameraView() : 
	m_view(sf::FloatRect(CAMERA_LEFT, CAMERA_TOP, CAMERA_WIDTH, CAMERA_HEIGHT)) {}

//----------------------------------functions section---------------------------------

// update the view according to the player's position, make sure not to leave the map
void CameraView::updateView(sf::Vector2f aladdinPos, int level)
{
	//specific camera views of both levels
	switch (level)
	{
	case LVL1:
	{
		if (aladdinPos.x <= 356.696)
			aladdinPos.x = 356.696;
		else if (aladdinPos.x >= 4421.59)
			aladdinPos.x = 4421.59;
		if (aladdinPos.y <= 278.421)
			aladdinPos.y = 278.421;
		else if (aladdinPos.y >= 491.611)
			aladdinPos.y = 491.611;
	}
	break;
	case LVL2:
	{
		if (aladdinPos.x <= 356.106)
			aladdinPos.x = 356.106;
		else if (aladdinPos.x >= 1911.03)
			aladdinPos.x = 1911.03;
		if (aladdinPos.y <= 205.944)
			aladdinPos.y = 205.944;
		else if (aladdinPos.y >= 947.325)
			aladdinPos.y = 947.325;
	}
	break;
	}
	

	m_view.setCenter(aladdinPos);
}

void CameraView::setView(sf::RenderWindow& window) const { window.setView(m_view); }

sf::View CameraView::getView() const { return m_view; }
