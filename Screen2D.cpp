#include "Screen2D.h"

Screen2D::Screen2D(CarGauge* carGauge)
{
	Screen2D::carGauge = carGauge;
}

Screen2D::~Screen2D()
{
	digital_counter.clean();
}

void Screen2D::setSize(int width, int height)
{
	Screen2D::width = width;
	Screen2D::height = height;
}

void Screen2D::loadFonts()
{
	digital_counter.init("data/digital_counter_7.ttf", 1.0f / 53 * height);
}

void Screen2D::display()
{
	pushScreenCoordinateMatrix();


	carGauge->display();
	//freetype::print(digital_counter, 0.5 * width + 0.522 * height, 0.174 * height, "%i", 5);
	//freetype::print(digital_counter, 100, 100, "55");
	freetype::display(digital_counter, 0.5 * width + 0.522 * height, 0.174 * height, "%i", 13);

	glPopAttrib();

	pop_projection_matrix();
}

void Screen2D::pushScreenCoordinateMatrix()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint   viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();
}

void Screen2D::pop_projection_matrix()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}