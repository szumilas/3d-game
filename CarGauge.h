#pragma once

#include "Car.h"

#include "TextureManager.h"
#include "GlobalStructures.h"
#include "enum.h"

class CarGauge
{

	friend class Screen2D;

public:

	CarGauge();
	void load(Car* car);
	void setScreenResolution(int w, int h);
	void display();

private:

	void calculateClockTipPositions();

public:


private:

	struct Clock
	{
		Point clockTipStartPosition;
		Point clockTipEndPosition;
		Point clockTipStartLeftPosition;
		Point clockTipStartRightPosition;

		float clockTipLenght;
		float clockTipWidth;

		float* currentValue;
		float maxValue;

		float ratio = 1.0f;

		void display()
		{
			glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex2f(clockTipStartLeftPosition.x, clockTipStartLeftPosition.y);
			glVertex2f(clockTipStartRightPosition.x, clockTipStartRightPosition.y);
			glVertex2f(clockTipEndPosition.x, clockTipEndPosition.y);

			glEnd();
		}

		void calculateTipPosition()
		{
			clockTipEndPosition = clockTipStartPosition;
			clockTipStartLeftPosition = clockTipStartPosition;
			clockTipStartRightPosition = clockTipStartPosition;

			float angle = 4.9 / 4.0 * PI;
			angle -= (*currentValue * ratio / maxValue) * 2.92 / 2.0 * PI;

			clockTipEndPosition.x += clockTipLenght * cos(angle);
			clockTipEndPosition.y += clockTipLenght * sin(angle);

			angle += PI / 2;
			clockTipStartLeftPosition.x += clockTipWidth * cos(angle);
			clockTipStartLeftPosition.y += clockTipWidth * sin(angle);

			angle -= PI;
			clockTipStartRightPosition.x += clockTipWidth * cos(angle);
			clockTipStartRightPosition.y += clockTipWidth * sin(angle);
		}
	};

	unsigned int idTexture;

	Car* car = nullptr;

	Clock velocityClock;
	Clock RPMClock;

	int w;
	int h;

};