#pragma once

#include "TextureManager.h"
#include "GlobalStructures.h"
#include "enum.h"

class CarGauge
{
public:

	CarGauge();
	void load(TextureManager* textureManager);
	void display();
	inline void setVelocity(float v /*[m/s]*/) { velocityClock.currentValue = 3.6 * abs(v); };
	inline void setRPM(float RPM /*[m/s]*/) { RPMClock.currentValue = RPM; };

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

		float clockTipLenght = 0.04f;
		float clockTipWidth = 0.002f;

		float currentValue;
		float maxValue;
		
		void display()
		{
			glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex3f(clockTipStartLeftPosition.x, clockTipStartLeftPosition.y, -0.5);
			glVertex3f(clockTipStartRightPosition.x, clockTipStartRightPosition.y, -0.5);
			glVertex3f(clockTipEndPosition.x, clockTipEndPosition.y, -0.5);
			glEnd();
		}

		void calculateTipPosition()
		{
			clockTipEndPosition = clockTipStartPosition;
			clockTipStartLeftPosition = clockTipStartPosition;
			clockTipStartRightPosition = clockTipStartPosition;

			float angle = 4.9 / 4.0 * PI;
			angle -= (currentValue / maxValue) * 2.92 / 2.0 * PI;

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

	Clock velocityClock;
	Clock RPMClock;

};