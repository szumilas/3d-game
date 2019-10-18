#include "Menu.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Screen2D.h"
#include "carDB.h"
#include "MapManager.h"

void display2DRectangle(Point& bottomLeft, Point& topRight, ColorName colorName = ColorName::WHITE, float z = 0)
{
	glBegin(GL_POLYGON);

	Menu::setGLcolor(colorName);

	glTexCoord2f(0, 0);
	glVertex3f(bottomLeft.x, bottomLeft.y, z);
	glTexCoord2f(1, 0);
	glVertex3f(topRight.x, bottomLeft.y, z);
	glTexCoord2f(1, 1);
	glVertex3f(topRight.x, topRight.y, z);
	glTexCoord2f(0, 1);
	glVertex3f(bottomLeft.x, topRight.y, z);

	glEnd();
}

void display2DLine(Point& p1, Point& p2, ColorName colorName = ColorName::WHITE, float size = 1, float z = 0)
{
	glLineWidth(size);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);

	Menu::setGLcolor(colorName);

	glVertex3f(p1.x, p1.y, z);
	glVertex3f(p2.x, p2.y, z);

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void display2DRectangleNoTexture(Point& bottomLeft, Point& topRight, ColorName colorName = ColorName::WHITE, float z = 0)
{
	glDisable(GL_TEXTURE_2D);

	display2DRectangle(bottomLeft, topRight, colorName, z);

	glEnable(GL_TEXTURE_2D);
}

void display2DRectangleTexture(Point& bottomLeft, Point& topRight, unsigned int idTexture, ColorName colorName = ColorName::WHITE, float z = 0)
{
	glBindTexture(GL_TEXTURE_2D, idTexture);

	display2DRectangle(bottomLeft, topRight, colorName, z);
}

void Menu::Init(int w, int h)
{
	idTextureBackground = TextureManager::Instance()->textures[static_cast<int>(Textures::menu_background)].idTexture;
	idTextureArrow = TextureManager::Instance()->textures[static_cast<int>(Textures::menu_arrow)].idTexture;
	idTextureWroclawMap = TextureManager::Instance()->textures[static_cast<int>(Textures::wroclaw_map)].idTexture;
	createMenu();
	Menu::w = w;
	Menu::h = h;

	exampleCar = std::make_unique<Car>(Car(CarBrand::SubaruBRZ, 0, 0));
	exampleTrack = std::make_unique<Track>(Track(BigLoop));
	//currentMenuLevel = &quickRace;
}

void Menu::displayBackground()
{
	glBindTexture(GL_TEXTURE_2D, idTextureBackground);
	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-9.78, -30, -18.15);
	glTexCoord2f(1, 0);
	glVertex3f(-9.78, 30, -18.15);
	glTexCoord2f(1, 1);
	glVertex3f(-17.54, 30, 10.83);
	glTexCoord2f(0, 1);
	glVertex3f(-17.54, -30, 10.83);

	glEnd();
}

void Menu::displayForegroundBeforeText()
{
	display2DRectangleTexture(Point(0.5 * w - 0.50 * h, 0.18 * h), Point(0.5 * w - 0.42 * h, 0.25 * h), idTextureArrow, currentMenuLevel->selected == 0 ? ColorName::MENU_LIGHT_GRAY : ColorName::YELLOW, 1);
	display2DRectangleTexture(Point(0.5 * w + 0.50 * h, 0.18 * h), Point(0.5 * w + 0.42 * h, 0.25 * h, 0.25 * h), idTextureArrow, currentMenuLevel->selected == currentMenuLevel->options.size() - 1 ? ColorName::MENU_LIGHT_GRAY : ColorName::YELLOW, 1);
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.18 * h), Point(0.5 * w - 0.37 * h, 0.25 * h), ColorName::MENU_DARK_GRAY, 1);
	display2DRectangleNoTexture(Point(0.5 * w + 0.55 * h, 0.18 * h), Point(0.5 * w + 0.37 * h, 0.25 * h), ColorName::MENU_DARK_GRAY, 1);
}

void Menu::displayForeground()
{
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.18 * h), Point(0.5 * w + 0.55 * h, 0.25 * h), ColorName::MENU_GRAY);
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.25 * h), Point(0.5 * w + 0.55 * h, 0.255 * h), ColorName::MENU_BLUE);
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.18 * h), Point(0.5 * w + 0.55 * h, 0.175 * h), ColorName::MENU_BLUE);
		
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -45, 27.5, currentMenuLevel->text, &(Screen2D::Instance()->wallpoet_regular));
	
	std::vector<std::tuple<ColorName, std::string, float>> textValuesToPrint;

	for (int i = currentMenuLevel->selected - 2; i <= currentMenuLevel->selected + 2; ++i)
	{
		ColorName colorName;
		if (i == currentMenuLevel->selected)
			colorName = ColorName::YELLOW;
		else
			colorName = ColorName::WHITE;

		std::string textToPrint;

		if (i >= 0 && i < currentMenuLevel->options.size())
		{
			textToPrint = currentMenuLevel->options[i]->text;
		}
		else
		{
			textToPrint = "===   ===";
			colorName = ColorName::MENU_LIGHT_GRAY;
		}

		textValuesToPrint.push_back({ colorName, textToPrint, 0 });
	}

	static float gap = 7;

	std::get<2>(textValuesToPrint[0]) = 0 - textRealSize(std::get<1>(textValuesToPrint[0]), Screen2D::Instance()->squada_one_regular_big.h) / 2
		- textRealSize(std::get<1>(textValuesToPrint[1]), Screen2D::Instance()->squada_one_regular_big.h)
		- textRealSize(std::get<1>(textValuesToPrint[2]), Screen2D::Instance()->squada_one_regular_big.h) / 2 - 2 * gap;
	std::get<2>(textValuesToPrint[1]) = 0 - textRealSize(std::get<1>(textValuesToPrint[1]), Screen2D::Instance()->squada_one_regular_big.h) / 2
		- textRealSize(std::get<1>(textValuesToPrint[2]), Screen2D::Instance()->squada_one_regular_big.h) / 2 - gap;
	std::get<2>(textValuesToPrint[2]) = 0;
	std::get<2>(textValuesToPrint[3]) = 0 + textRealSize(std::get<1>(textValuesToPrint[2]), Screen2D::Instance()->squada_one_regular_big.h) / 2
		+ textRealSize(std::get<1>(textValuesToPrint[3]), Screen2D::Instance()->squada_one_regular_big.h) / 2 + gap;
	std::get<2>(textValuesToPrint[4]) = 0 + textRealSize(std::get<1>(textValuesToPrint[2]), Screen2D::Instance()->squada_one_regular_big.h) / 2
		+ textRealSize(std::get<1>(textValuesToPrint[3]), Screen2D::Instance()->squada_one_regular_big.h)
		+ textRealSize(std::get<1>(textValuesToPrint[4]), Screen2D::Instance()->squada_one_regular_big.h) / 2 + 2 * gap;


	for (auto& textValueToPrint : textValuesToPrint)
	{
		float x = std::get<2>(textValueToPrint);
		float xPos = x;
		if (xPos - textRealSize(std::get<1>(textValueToPrint), Screen2D::Instance()->squada_one_regular_big.h) / 2 + textMenuOffset * 25 > -52
			&& xPos + textRealSize(std::get<1>(textValueToPrint), Screen2D::Instance()->squada_one_regular_big.h) / 2 + textMenuOffset * 25 < 52)
		{
			xPos = centerFont(x, std::get<1>(textValueToPrint), Screen2D::Instance()->squada_one_regular_big.h);
			xPos += textMenuOffset * 25;
			Screen2D::Instance()->addTestValueToPrint(std::get<0>(textValueToPrint), xPos, 20, std::get<1>(textValueToPrint), &(Screen2D::Instance()->squada_one_regular_big));
		}
	}
}

void Menu::createMenu()
{
	mainMenu.options = {&quickRace, &freeRide, &highscores, &credits, &quitGame };
	quickRace.options = { &quickRaceStart, &quickRaceSelectCar, &quickRaceSelectTrack, &quickRaceBack };
	freeRide.options = { &freeRideStart, &freeRideSelectCar, &freeRideBack };
	highscores.options = { &highscoresRideBack };
	credits.options = { &creditsRideBack };

	for (auto& car : carDB)
	{
		carOptions.push_back({ car.second.name, &Menu::enterPreviousLevel, &Menu::preview2DCar, &Menu::preview3DCar, static_cast<int>(car.first) });
	}

	carOptions.push_back({ "Cancel", &Menu::enterPreviousLevel });
	
	for (auto& carOption : carOptions)
	{
		quickRaceSelectCar.options.push_back(&carOption);
	}


	for (auto& track : trackDB)
	{
		trackOptions.push_back({ track.second, &Menu::enterPreviousLevel, &Menu::preview2DTrack, &Menu::preview3DTrack, static_cast<int>(track.first) });
	}

	trackOptions.push_back({ "Cancel", &Menu::enterPreviousLevel });

	for (auto& trackOption : trackOptions)
	{
		quickRaceSelectTrack.options.push_back(&trackOption);
	}


	currentMenuLevel = &mainMenu;
}

void Menu::selectPrevious()
{
	currentMenuLevel->selected--;
	if (currentMenuLevel->selected < 0)
		currentMenuLevel->selected = 0;
}

void Menu::selectNext()
{
	currentMenuLevel->selected++;
	if (currentMenuLevel->selected >= currentMenuLevel->options.size())
		currentMenuLevel->selected = currentMenuLevel->options.size() - 1;
}

void Menu::enter()
{
	if (currentMenuLevel->options[currentMenuLevel->selected]->execute != nullptr)
	{
		(this->*currentMenuLevel->options[currentMenuLevel->selected]->execute)();
	}
}

float Menu::centerFont(float originalXpercent, std::string text, float fontSize)
{
	return originalXpercent - textRealSize(text, fontSize) / 2;
}

float Menu::textRealSize(std::string text, float fontSize)
{
	return 100.0 * text.size() / 2 * fontSize / h;
}

void Menu::update()
{
	static float floatingIndex = 0;
	
	float acceleration = pow(abs(static_cast<float>(currentMenuLevel->selected) - floatingIndex) + 0.1f, 2) / 3;

	if (currentMenuLevel->selected > floatingIndex)
	{
		floatingIndex += acceleration;
	}
	else if (currentMenuLevel->selected < floatingIndex)
	{
		floatingIndex -= acceleration;
	}

	if (abs(static_cast<float>(currentMenuLevel->selected) - floatingIndex) < 0.006)
	{
		floatingIndex = static_cast<float>(currentMenuLevel->selected);
	}

	textMenuOffset = static_cast<float>(currentMenuLevel->selected) - floatingIndex;
}

void Menu::enterNextLevel()
{
	int selectedOption = currentMenuLevel->selected;
	MenuLevel* preiousLevel = currentMenuLevel;
	currentMenuLevel->selected = 0;
	currentMenuLevel = currentMenuLevel->options[selectedOption];
	currentMenuLevel->previousLevel = preiousLevel;
	currentMenuLevel->selected = 0;
}

void Menu::enterPreviousLevel()
{
	currentMenuLevel = currentMenuLevel->previousLevel;
	currentMenuLevel->selected = 0;
}

void Menu::display3DscreenForOption()
{
	if (currentMenuLevel->options[currentMenuLevel->selected]->preview3D != nullptr)
	{
		(this->*currentMenuLevel->options[currentMenuLevel->selected]->preview3D)(currentMenuLevel->options[currentMenuLevel->selected]->idOption);
	}
}

void Menu::display2DscreenForOption()
{
	if (currentMenuLevel->options[currentMenuLevel->selected]->preview2D != nullptr)
	{
		(this->*currentMenuLevel->options[currentMenuLevel->selected]->preview2D)(currentMenuLevel->options[currentMenuLevel->selected]->idOption);
	}
}

void Menu::preview2DCar(int id)
{
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.35 * h), Point(0.5 * w - 0.25 * h, 0.75 * h), ColorName::MENU_GRAY);
	display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.32 * h), Point(0.5 * w - 0.25 * h, 0.35 * h), ColorName::MENU_BLUE);

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 70, "Engine power: ", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 66, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).power * 1.36)) + " HP", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 61, "Mass: ", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 57, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).mass)) + " kg", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 52, "Top speed: ", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 48, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).vMax)) + " km/h", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 43, "Acceleration 0-100 km/h: ", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 39, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).acceleration_0_100)) + " s", &(Screen2D::Instance()->squada_one_regular_big));
}

void Menu::preview3DCar(int id)
{
	static float angle = 0;
	angle += PI / 6 / FPS;

	if (id != static_cast<int>(exampleCar->getCarBrand()))
	{
		exampleCar = std::make_unique<Car>(Car(static_cast<CarBrand>(id), 0, 0));
	}

	exampleCar->setPosition(Point(0, 1.5), angle);
	exampleCar->display();
	exampleCar->alreadyPrinted = false;
}

void Menu::preview2DTrack(int id)
{
	//display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.35 * h), Point(0.5 * w - 0.25 * h, 0.75 * h), ColorName::MENU_GRAY);
	//display2DRectangleNoTexture(Point(0.5 * w - 0.55 * h, 0.32 * h), Point(0.5 * w - 0.25 * h, 0.35 * h), ColorName::MENU_BLUE);
	//
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 70, "Engine power: ", &(Screen2D::Instance()->squada_one_regular));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 66, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).power * 1.36)) + " HP", &(Screen2D::Instance()->squada_one_regular_big));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 61, "Mass: ", &(Screen2D::Instance()->squada_one_regular));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 57, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).mass)) + " kg", &(Screen2D::Instance()->squada_one_regular_big));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 52, "Top speed: ", &(Screen2D::Instance()->squada_one_regular));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 48, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).vMax)) + " km/h", &(Screen2D::Instance()->squada_one_regular_big));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 43, "Acceleration 0-100 km/h: ", &(Screen2D::Instance()->squada_one_regular));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -52, 39, std::to_string(static_cast<int>(carDB.at(static_cast<CarBrand>(id)).acceleration_0_100)) + " s", &(Screen2D::Instance()->squada_one_regular_big));
}

void Menu::preview3DTrack(int id)
{
	static float angle = 0;
	angle += PI / 12 / FPS;

	if (id != static_cast<int>(exampleTrack->getTrackName()))
	{
		exampleTrack = std::make_unique<Track>(Track(static_cast<TrackName>(id)));
	}

	exampleTrack->setPosition(Point(0, 1.5), angle);
	exampleTrack->display();
	exampleTrack->alreadyPrinted = false;
}

void Menu::quickRace2Dpreview(int id)
{
	display2DRectangleNoTexture(screenPoint(5, 38), screenPoint(55, 65), ColorName::MENU_GRAY);
	display2DRectangleNoTexture(screenPoint(5, 35), screenPoint(55, 38), ColorName::MENU_BLUE);

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 7, 60, "Car:", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 12, 60, carDB.at(exampleCar->getCarBrand()).name, &(Screen2D::Instance()->squada_one_regular_big));

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 7, 55, "Track:", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 14.5, 55, "University of Technology", &(Screen2D::Instance()->squada_one_regular_big));

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 7, 50, "No. of laps:", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 19, 50, "4", &(Screen2D::Instance()->squada_one_regular_big));

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 7, 45, "Total length:", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 20.5, 45, "15.78 km", &(Screen2D::Instance()->squada_one_regular_big));

	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 7, 40, "No. of oponents:", &(Screen2D::Instance()->squada_one_regular));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 24, 40, "6", &(Screen2D::Instance()->squada_one_regular_big));


	printMap();

}

void Menu::quickRace3Dpreview(int id)
{
	static float angle = 0;
	angle += PI / 6 / FPS;

	exampleCar->setPosition(Point(-10, 6, 1), angle);
	exampleCar->display();
	exampleCar->alreadyPrinted = false;

}

void Menu::printMap()
{
	static Point leftBottomMapPoint = screenPoint(-55, 40);
	static Point rightTopMapPoint = screenPoint(0, 78.5);
	static Point centerMap = { (leftBottomMapPoint.x + rightTopMapPoint.x) / 2, (leftBottomMapPoint.y + rightTopMapPoint.y) / 2 };
	static Point centerMapCoordinates = { 17.056488, 51.112627 };

	display2DRectangleTexture(leftBottomMapPoint, rightTopMapPoint, idTextureWroclawMap);


	Menu::setGLcolor(ColorName::ORANGE);

	auto calculateScreenPointOnMap = [](Point& p1)
	{
		Point p2(p1.x - centerMapCoordinates.x, p1.y - centerMapCoordinates.y);
		p2.x *= MapManager::longituteRatio;
		p2.y *= MapManager::latitudeRatio;
		p2.x /= 42.128;
		p2.y /= 42.128;

		p2.x *= 10.9;
		p2.y *= 10.9;
		p2 += centerMap;

		return p2;
	};


	for (int q = 0; q < exampleTrack->AIPointsCoordinates.size(); q++)
	{
		Point& p1 = exampleTrack->AIPointsCoordinates[q];
		Point& p2 = exampleTrack->AIPointsCoordinates[(q + 1) % exampleTrack->AIPointsCoordinates.size()];

		display2DLine(calculateScreenPointOnMap(p1), calculateScreenPointOnMap(p2), ColorName::MENU_BLUE, 3, 1);
	}

}