#pragma once

#include "TextureManager.h"
#include "GlobalStructures.h"
#include "MapContainer.h"
#include "carDB.h"
#include "Car.h"

class Menu
{
public:

	void Init(int w, int h);
	void displayBackground();
	void displayForeground();
	void displayForegroundBeforeText();
	void display3DscreenForOption();
	void display2DscreenForOption();

	void update();

	void selectPrevious();
	void selectNext();
	void enter();

	static void setGLcolor(ColorName colorName)
	{
		Color color = Color(colorName);
		glColor3f(color.red, color.green, color.blue);
	}

private:

	void createMenu();

	float centerFont(float originalXpercent, std::string text, float fontSize);
	float textRealSize(std::string text, float fontSize); //[%]

	void enterNextLevel();
	void enterPreviousLevel();
	void preview2DCar(int id);
	void preview3DCar(int id);

public:


private:
	
	unsigned int idTextureBackground;
	unsigned int idTextureArrow;

	int w;
	int h;

	float textMenuOffset = 0;

	std::unique_ptr<Car> exampleCar;


	struct MenuLevel
	{
		std::string text;
		void(Menu::*execute)() = nullptr;
		void(Menu::*preview2D)(int) = nullptr;
		void(Menu::*preview3D)(int) = nullptr;
		int idOption = 0;
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