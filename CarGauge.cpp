#include "CarGauge.h"

CarGauge::CarGauge()
{
	velocityClock.currentValue = 0;
	
	RPMClock.maxValue = 8000;
	RPMClock.currentValue = 0;
}

void CarGauge::setScreenResolution(int w, int h)
{
	CarGauge::w = w;
	CarGauge::h = h;

	velocityClock.clockTipStartPosition = { 0.5 * w + 0.4996 * h, 0.105 * h };
	velocityClock.clockTipLenght = 0.08f * h;
	velocityClock.clockTipWidth = 0.004f * h;
	velocityClock.currentValue = &car->velocity;
	velocityClock.ratio = 3.6f;

	RPMClock.clockTipStartPosition = { 0.5 * w + 0.2996 * h, 0.105 * h };
	RPMClock.clockTipLenght = 0.08f * h;
	RPMClock.clockTipWidth = 0.004f * h;
	RPMClock.currentValue = &car->engine.RPM;
}

void CarGauge::load(Car* car)
{
	if (car->vMax > 220 / 3.6)
	{
		idTexture = Game::textureManager.textures[static_cast<int>(Textures::car_gauge_330)].idTexture;
		velocityClock.maxValue = 330;
	}
	else
	{
		idTexture = Game::textureManager.textures[static_cast<int>(Textures::car_gauge)].idTexture;
		velocityClock.maxValue = 220;
	}
	CarGauge::car = car;
}

void CarGauge::display()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);



	calculateClockTipPositions();
	RPMClock.display();
	velocityClock.display();




	glBindTexture(GL_TEXTURE_2D, idTexture);
	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex2f(0.5 * w + 0.2 * h, 0);
	glTexCoord2f(1, 0);
	glVertex2f(0.5 * w + 0.6 * h, 0);
	glTexCoord2f(1, 1);
	glVertex2f(0.5 * w + 0.6 * h, 0.2 * h);
	glTexCoord2f(0, 1);
	glVertex2f(0.5 * w + 0.2 * h, 0.2 * h);

	glEnd();


	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	Game::screen2D.addTestValueToPrint(ColorName::RED, 49.5, 3.5, std::to_string(car->getCurrentGear()), &Game::screen2D.digital_counter);
}

void CarGauge::calculateClockTipPositions()
{
	velocityClock.calculateTipPosition();
	RPMClock.calculateTipPosition();
}