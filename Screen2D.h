#pragma once

#include "freetype.h"
#include "CarGauge.h"

class Screen2D
{
public:

	Screen2D(CarGauge* carGauge);
	~Screen2D();

	void setSize(int width, int height);
	void loadFonts();
	void display();

private:

	void pushScreenCoordinateMatrix();
	void pop_projection_matrix();

public:



private:

	int width;
	int height;
	CarGauge* carGauge;


	freetype::font_data digital_counter;

};