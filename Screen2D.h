#pragma once

#include "GlobalStructures.h"
#include "freetype.h"

class Screen2D
{

	struct Screen2Dtext
	{
		ColorName colorName;
		float x; //[%]
		float y; //[%]
		std::string text;
		freetype::font_data* font;
	};

public:

	~Screen2D();

	void setSize(int width, int height);
	void loadFonts();
	void display();
	void addTestValueToPrint(ColorName colorName, float xp, float yp, std::string text, freetype::font_data* font)
	{
		textValues.push_back({ colorName, xp, yp, text, font });
	}
	static void pushScreenCoordinateMatrix(); 
	static void pop_projection_matrix(); 

private:


public:

	freetype::font_data digital_counter;
	freetype::font_data stocky;
	freetype::font_data roboto_modo_regular;


private:

	int width;
	int height;


	std::vector<Screen2Dtext> textValues;

};