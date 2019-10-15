#pragma once

#include "TextureManager.h"
#include "GlobalStructures.h"

class Menu
{
public:

	void Init(int w, int h);
	void displayBackground();
	void displayForeground();
	void displayForegroundBeforeText();

	void update();

	void selectPrevious();
	void selectNext();
	void enter();

	void enterNextLevel();
	void enterPreviousLevel();

private:

	void createMenu();
	static void setGLcolor(ColorName colorName)
	{
		Color color = Color(colorName);
		glColor3f(color.red, color.green, color.blue);
	}

	float centerFont(float originalXpercent, std::string text, float fontSize);
	float textRealSize(std::string text, float fontSize); //[%]

public:


private:
	
	unsigned int idTextureBackground;
	unsigned int idTextureArrow;

	int w;
	int h;

	float textMenuOffset = 0;


	struct MenuLevel
	{
		std::string text;
		void(Menu::*function)() = nullptr;
		std::vector<MenuLevel*> options;
		int selected = 0;
		MenuLevel* previousLevel = nullptr;
	};

	MenuLevel* currentMenuLevel = nullptr;
	std::vector<MenuLevel> allMenuOptions;

	MenuLevel mainMenu{ "Main Menu" };
	MenuLevel quickRace{ "Quick Race", &Menu::enterNextLevel };
	MenuLevel freeRide{ "Free Ride", &Menu::enterNextLevel };
	MenuLevel highscores{ "Highscores", &Menu::enterNextLevel };
	MenuLevel credits{ "Credits", &Menu::enterNextLevel };
	MenuLevel quitGame{ "Quit Game" };

	MenuLevel quickRaceStart{ "Start" };
	MenuLevel quickRaceSelectCar{ "Select Car", &Menu::enterNextLevel };
	MenuLevel quickRaceSelectTrack{ "Select Track", &Menu::enterNextLevel };
	MenuLevel quickRaceBack{ "Back", &Menu::enterPreviousLevel };

	MenuLevel freeRideStart{ "Start" };
	MenuLevel freeRideSelectCar{ "Select Car", &Menu::enterNextLevel };
	MenuLevel freeRideBack{ "Back", &Menu::enterPreviousLevel };

	MenuLevel highscoresRideBack{ "Back", &Menu::enterPreviousLevel };

	MenuLevel creditsRideBack{ "Back", &Menu::enterPreviousLevel };

	std::vector<MenuLevel> carOptions;

};