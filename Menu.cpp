#include "Menu.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Screen2D.h"

void Menu::Init()
{
	idTexture = TextureManager::Instance()->textures[static_cast<int>(Textures::menu_background)].idTexture;
}

void Menu::displayBackground()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, idTexture);
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
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -50, 20, "Quick Race", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, -20, 20, "Free Ride", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 10, 20, "Highscores", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 40, 20, "Credits", &(Screen2D::Instance()->squada_one_regular_big));
	Screen2D::Instance()->addTestValueToPrint(ColorName::WHITE, 70, 20, "Quit", &(Screen2D::Instance()->squada_one_regular_big));
}