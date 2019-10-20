#pragma once

#include "TextureManager.h"
#include "GlobalStructures.h"
#include "MapContainer.h"
#include "carDB.h"
#include "Car.h"
#include "Track.h"

class Menu
{
public:

	enum MenuState
	{
		OK = 1,
		StartQuickRace,
	};

	enum class MapDetails
	{
		Track,
		Position,
	};

	struct MenuResponse
	{
		MenuState menuState = OK;
		CarBrand selectedCar;
		TrackName selectedTrack;
		int noOfLaps;
		int noOfOponents;
	};

public:

	void Init(int w, int h);
	void displayBackground();
	void displayForeground();
	void displayForegroundBeforeText();
	void display3DscreenForOption();
	void display2DscreenForOption();

	bool update();

	void selectPrevious();
	void selectNext();
	void enter();
	void loadAfterRaceScreen();

	static void setGLcolor(ColorName colorName)
	{
		Color color = Color(colorName);
		glColor3f(color.red, color.green, color.blue);
	}

private:

	void createMenu();
	void createCommonOptions();

	float centerFont(float originalXpercent, std::string text, float fontSize);
	float textRealSize(std::string text, float fontSize); //[%]

	void enterNextLevel();
	void selectThisCar();
	void selectThisTrack();
	void selectThisNoOfOponents();
	void selectThisNoOfLaps();
	void selectThisFreeRidePosition();
	void enterPreviousLevel();
	void enterQuickRaceLevel();
	void startQuickRace();
	void startFreeRide();

	void preview2DNumber(int id);
	void preview2DFreeRidePositions(int id);
	void preview2DCar(int id);
	void preview3DCar(int id);
	void quickRace2Dpreview(int id = 0);
	void quickRace3Dpreview(int id = 0);
	void freeRide2Dpreview(int id = 0);
	void freeRide3Dpreview(int id = 0);
	void preview2DTrack(int id);
	void preview3DTrack(int id);
	void preview2DQuickRaceStats(int id);
	void preview2DQuickRaceResults(int id);
	void preview2DQuickRaceLaps(int id);

	void printMap(MapDetails details);
	void reloadQuickRaceData();

	Point screenPoint(float xp, float yp) { return Point(0.5 * w + xp / 100 * h, yp / 100 * h); }

public:

	MenuResponse menuResponse;

private:
	
	unsigned int idTextureBackground;
	unsigned int idTextureArrow;
	unsigned int idTextureWroclawMap;

	int w;
	int h;

	float textMenuOffset = 0;
	float floatingIndex = 0;


	std::unique_ptr<Car> previewCar;
	std::unique_ptr<Track> previewTrack;
	int previewNumber;

	CarBrand selectedCar = CarBrand::SubaruBRZ;
	TrackName selectedTrack = SmallLoop;
	int selectedNoOfOponents = 4;
	int selectedNoOfLaps = 2;
	int selectedFreeRidePosition = 1;

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

	MenuLevel quickRaceStart{ "Start", &Menu::startQuickRace, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceSelectCar{ "Select Car", &Menu::enterNextLevel, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceSelectTrack{ "Select Track", &Menu::enterNextLevel, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceNoOfLaps{ "No. Of Laps", &Menu::enterNextLevel, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceNoOfOponents{ "No. Of Oponents", &Menu::enterNextLevel, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceBack{ "Back", &Menu::enterPreviousLevel, &Menu::quickRace2Dpreview, &Menu::quickRace3Dpreview };
	MenuLevel quickRaceAfterRaceScreen{ "Race Finished" };
	MenuLevel quickRaceAfterRaceScreenBack{ "Continue", &Menu::enterQuickRaceLevel, &Menu::preview2DQuickRaceStats };
	MenuLevel quickRaceResults{ "Results", &Menu::enterNextLevel, &Menu::preview2DQuickRaceStats };
	MenuLevel quickRaceResultsBack{ "Back", &Menu::enterPreviousLevel, &Menu::preview2DQuickRaceResults };
	MenuLevel quickRaceLaps{ "Laps", &Menu::enterNextLevel, &Menu::preview2DQuickRaceStats };
	MenuLevel quickRaceLapsBack{ "Back", &Menu::enterPreviousLevel, &Menu::preview2DQuickRaceLaps };

	MenuLevel freeRideStart{ "Start", &Menu::startFreeRide, &Menu::freeRide2Dpreview, &Menu::freeRide3Dpreview };
	MenuLevel freeRideSelectCar{ "Select Car", &Menu::enterNextLevel, &Menu::freeRide2Dpreview, &Menu::freeRide3Dpreview };
	MenuLevel freeRideSelectPosition{ "Select Position", &Menu::enterNextLevel, &Menu::freeRide2Dpreview, &Menu::freeRide3Dpreview };
	MenuLevel freeRideBack{ "Back", &Menu::enterPreviousLevel, &Menu::freeRide2Dpreview, &Menu::freeRide3Dpreview };

	MenuLevel highscoresRideBack{ "Back", &Menu::enterPreviousLevel };

	MenuLevel creditsRideBack{ "Back", &Menu::enterPreviousLevel };

	std::vector<MenuLevel> carOptions;
	std::vector<MenuLevel> trackOptions;
	std::vector<MenuLevel> numberOfLaps;
	std::vector<MenuLevel> numberOfOponents;

	std::vector<MenuLevel> freeRidePositions;

	std::map<int, std::pair<std::string, Point>> freeRideStartPositions =
	{
		{ 1, { "Grunwald Square", Point(17.061105, 51.111835) } },
		{ 2, { "Botanical Garden", Point(17.051355, 51.116598) } },
		{ 3, { "Green Day", Point(17.051100, 51.113702) } },
		{ 4, { "University", Point(17.061078, 51.107167) } },
		{ 5, { "Sienkiewicza St.", Point(17.058047, 51.116090) } },
		{ 6, { "Reja St.", Point(17.056846, 51.113148) } },
	};
};