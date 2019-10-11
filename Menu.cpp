#include "Menu.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Screen2D.h"

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

void Menu::displayForeground()
{

	glBindTexture(GL_TEXTURE_2D, idTextureArrow);
	glBegin(GL_POLYGON);

	if (currentMenuLevel->selected == 0)
		setGLcolor(ColorName::GRAY_MENU);
	else
		setGLcolor(ColorName::YELLOW);
	
	glTexCoord2f(0, 0);
	glVertex2f(0.5 * w - 0.50 * h, 0.18 * h);
	glTexCoord2f(1, 0);
	glVertex2f(0.5 * w - 0.42 * h, 0.18 * h);
	glTexCoord2f(1, 1);
	glVertex2f(0.5 * w - 0.42 * h, 0.25 * h);
	glTexCoord2f(0, 1);
	glVertex2f(0.5 * w - 0.50 * h, 0.25 * h);

	glEnd();

	//----------------------

	glBindTexture(GL_TEXTURE_2D, idTextureArrow);
	glBegin(GL_POLYGON);

	if (currentMenuLevel->selected == currentMenuLevel->options.size() - 1)
		setGLcolor(ColorName::GRAY_MENU);
	else
		setGLcolor(ColorName::YELLOW);

	glTexCoord2f(0, 0);
	glVertex2f(0.5 * w + 0.50 * h, 0.18 * h);
	glTexCoord2f(1, 0);
	glVertex2f(0.5 * w + 0.42 * h, 0.18 * h);
	glTexCoord2f(1, 1);
	glVertex2f(0.5 * w + 0.42 * h, 0.25 * h);
	glTexCoord2f(0, 1);
	glVertex2f(0.5 * w + 0.50 * h, 0.25 * h);

	glEnd();

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.23f, 0.23f);

	glVertex2f(0.5 * w - 0.5 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.25 * h);
	glVertex2f(0.5 * w - 0.5 * h, 0.25 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.176f, 0.749f, 0.851f);

	glVertex2f(0.5 * w - 0.5 * h, 0.25 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.25 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.255 * h);
	glVertex2f(0.5 * w - 0.5 * h, 0.255 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glColor3f(0.176f, 0.749f, 0.851f);

	glVertex2f(0.5 * w - 0.5 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.18 * h);
	glVertex2f(0.5 * w + 0.5 * h, 0.175 * h);
	glVertex2f(0.5 * w - 0.5 * h, 0.175 * h);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	//----------------------
	
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -45, 27.5, currentMenuLevel->text, &(Screen2D::Instance()->wallpoet_regular));

	int x = -50;

	for (int i = currentMenuLevel->selected - 2; i <= currentMenuLevel->selected + 2; ++i)
	{
		ColorName colorName;
		if (i == currentMenuLevel->selected)
			colorName = ColorName::YELLOW;
		else
			colorName = ColorName::WHITE;

		std::string textToPrint;
		float xPos;

		if (i >= 0 && i < currentMenuLevel->options.size())
		{
			textToPrint = currentMenuLevel->options[i]->text;
		}
		else
		{
			textToPrint = "===   ===";
			colorName = ColorName::GRAY_MENU;
		}

		xPos = x + textMenuOffset * 25;
		if (xPos > -35 && xPos < 35)
		{
			xPos = centerFont(x, textToPrint.size(), Screen2D::Instance()->squada_one_regular_big.h);
			xPos += textMenuOffset * 25;
			Screen2D::Instance()->addTestValueToPrint(colorName, xPos, 20, textToPrint, &(Screen2D::Instance()->squada_one_regular_big));
		}

		x += 25;
	}
}

void Menu::createMenu()
{
	mainMenu.options = {&quickRace, &freeRide, &highscores, &credits, &quitGame };

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

float Menu::centerFont(float originalXpercent, int textLength, float fontSize)
{
	return originalXpercent - 100.0 * textLength / 2 * fontSize / h / 2;
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