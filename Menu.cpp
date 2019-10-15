#include "Menu.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Screen2D.h"
#include "carDB.h"

void Menu::Init(int w, int h)
{
	idTextureBackground = TextureManager::Instance()->textures[static_cast<int>(Textures::menu_background)].idTexture;
	idTextureArrow = TextureManager::Instance()->textures[static_cast<int>(Textures::menu_arrow)].idTexture;
	createMenu();
	Menu::w = w;
	Menu::h = h;
}

void Menu::displayBackground()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

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

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Menu::displayForegroundBeforeText()
{
	glBindTexture(GL_TEXTURE_2D, idTextureArrow);
	glBegin(GL_POLYGON);

	if (currentMenuLevel->selected == 0)
		setGLcolor(ColorName::GRAY_MENU);
	else
		setGLcolor(ColorName::YELLOW);

	glTexCoord2f(0, 0);
	glVertex3f(0.5 * w - 0.50 * h, 0.18 * h, 1);
	glTexCoord2f(1, 0);
	glVertex3f(0.5 * w - 0.42 * h, 0.18 * h, 1);
	glTexCoord2f(1, 1);
	glVertex3f(0.5 * w - 0.42 * h, 0.25 * h, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5 * w - 0.50 * h, 0.25 * h, 1);

	glEnd();

	//----------------------

	glBindTexture(GL_TEXTURE_2D, idTextureArrow);
	glBegin(GL_POLYGON);

	if (currentMenuLevel->selected == currentMenuLevel->options.size() - 1)
		setGLcolor(ColorName::GRAY_MENU);
	else
		setGLcolor(ColorName::YELLOW);

	glTexCoord2f(0, 0);
	glVertex3f(0.5 * w + 0.50 * h, 0.18 * h, 1);
	glTexCoord2f(1, 0);
	glVertex3f(0.5 * w + 0.42 * h, 0.18 * h, 1);
	glTexCoord2f(1, 1);
	glVertex3f(0.5 * w + 0.42 * h, 0.25 * h, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5 * w + 0.50 * h, 0.25 * h, 1);

	glEnd();

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);

	glColor3f(0.15f, 0.17f, 0.18f);

	glTexCoord2f(0, 0);
	glVertex3f(0.5 * w - 0.55 * h, 0.18 * h, 1);
	glTexCoord2f(1, 0);
	glVertex3f(0.5 * w - 0.37 * h, 0.18 * h, 1);
	glTexCoord2f(1, 1);
	glVertex3f(0.5 * w - 0.37 * h, 0.25 * h, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5 * w - 0.55 * h, 0.25 * h, 1);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//---------------------------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);

	glColor3f(0.15f, 0.17f, 0.18f);

	glTexCoord2f(0, 0);
	glVertex3f(0.5 * w + 0.55 * h, 0.18 * h, 1);
	glTexCoord2f(1, 0);
	glVertex3f(0.5 * w + 0.37 * h, 0.18 * h, 1);
	glTexCoord2f(1, 1);
	glVertex3f(0.5 * w + 0.37 * h, 0.25 * h, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5 * w + 0.55 * h, 0.25 * h, 1);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//---------------------------------------
}

void Menu::displayForeground()
{


	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.23f, 0.23f);

	glVertex2f(0.5 * w - 0.55 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.25 * h);
	glVertex2f(0.5 * w - 0.55 * h, 0.25 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.176f, 0.749f, 0.851f);

	glVertex2f(0.5 * w - 0.55 * h, 0.25 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.25 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.255 * h);
	glVertex2f(0.5 * w - 0.55 * h, 0.255 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.176f, 0.749f, 0.851f);

	glVertex2f(0.5 * w - 0.55 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.55 * h, 0.175 * h);
	glVertex2f(0.5 * w - 0.55 * h, 0.175 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------
	
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
			colorName = ColorName::GRAY_MENU;
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
		carOptions.push_back({ car.second.name, &Menu::enterPreviousLevel });
	}
	
	for (auto& carOption : carOptions)
	{
		quickRaceSelectCar.options.push_back(&carOption);
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
	if (currentMenuLevel->options[currentMenuLevel->selected]->function != nullptr)
	{
		(this->*currentMenuLevel->options[currentMenuLevel->selected]->function)();
	}
	//if (currentMenuLevel->options[currentMenuLevel->selected]->text == "Back")
	//{
	//	enterPreviousLevel();
	//}
	//else
	//{
	//	if (!currentMenuLevel->options[currentMenuLevel->selected]->options.empty())
	//	{
	//		enterNextLevel();
	//	}
	//}
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