#pragma once

#include "GlobalStructures.h"
#include "freetype.h"

class Screen2D
{

	struct Screen2Dtext
	{
		ColorName colorName;
		int x; //[%]
		int y; //[%]
		std::string text;
	};

public:

	~Screen2D();

	void setSize(int width, int height);
	void loadFonts();
	void display();
	void addTestValueToPrint(ColorName colorName, int xp, int yp, std::string text)
	{
		testValues.push_back({ colorName, xp, yp, text });
	}

private:

	void pushScreenCoordinateMatrix();
	void pop_projection_matrix();

public:



private:

	int width;
	int height;


	freetype::font_data digital_counter;
	freetype::font_data stocky;
	freetype::font_data roboto_modo_regular;

	std::vector<Screen2Dtext> testValues;

};