#pragma once

#include <memory>

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
	static void Init(int width, int height);
	static std::unique_ptr<Screen2D>& Instance();

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

	Screen2D(int width, int height);

public:

	freetype::font_data digital_counter;
	freetype::font_data stocky;
	freetype::font_data roboto_modo_regular;


private:

	int width;
	int height;


	std::vector<Screen2Dtext> textValues;

	static std::unique_ptr<Screen2D> _instance;

};