#pragma once

#include "TextureManager.h"
#include "GlobalStructures.h"

class Menu
{
public:

	void Init(int w, int h);
	void displayBackground();
	void displayForeground();

	void selectPrevious();
	void selectNext();

private:

	void createMenu();
	static void setGLcolor(ColorName colorName)
	{
		Color color = Color(colorName);
		glColor3f(color.red, color.green, color.blue);
	}

	float centerFont(float originalXpercent, int textLength, float fontSize);

public:


private:
	
	unsigned int idTextureBackground;
	unsigned int idTextureArrow;

	int w;
	int h;


	struct MenuLevel
	{
		std::string text;
		std::vector<MenuLevel*> options;
		int selected = 0;
	};

	MenuLevel* currentMenuLevel = nullptr;
	std::vector<MenuLevel> allMenuOptions;

	MenuLevel mainMenu{ "Main Menu" };
	MenuLevel quickRace{ "Quick Race" };
	MenuLevel freeRide{ "Free Ride" };
	MenuLevel highscores{ "Highscores" };
	MenuLevel credits{ "Credits" };
	MenuLevel quitGame{ "Quit Game" };

};