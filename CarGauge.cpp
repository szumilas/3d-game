#include "CarGauge.h"

CarGauge::CarGauge()
{
	velocityClock.clockTipStartPosition = { 0.3496, -0.1482 };
	velocityClock.maxValue = 220;
	velocityClock.currentValue = 0;
	
	RPMClock.clockTipStartPosition = { 0.2496, -0.1482 };
	RPMClock.maxValue = 8000;
	RPMClock.currentValue = 0;
}

void CarGauge::load(TextureManager* textureManager)
{
	idTexture = textureManager->textures[static_cast<int>(Textures::car_gauge)].idTexture;
}

void CarGauge::display()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, idTexture);

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glTexCoord2f(0, 0);
	glVertex3f(0.2, -0.2, -0.501);
	glTexCoord2f(1, 0);
	glVertex3f(0.4, -0.2, -0.501);
	glTexCoord2f(1, 1);
	glVertex3f(0.4, -0.1, -0.501);
	glTexCoord2f(0, 1);
	glVertex3f(0.2, -0.1, -0.501);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	calculateClockTipPositions();
	velocityClock.display();
	RPMClock.display();
}

void CarGauge::calculateClockTipPositions()
{
	velocityClock.calculateTipPosition();
	RPMClock.calculateTipPosition();
}