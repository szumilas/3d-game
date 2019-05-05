#include "Screen2D.h"

Screen2D::~Screen2D()
{
	digital_counter.clean();
	stocky.clean();
}

void Screen2D::setSize(int width, int height)
{
	Screen2D::width = width;
	Screen2D::height = height;
}

void Screen2D::loadFonts()
{
	digital_counter.init("data/digital_counter_7.ttf", 1.0f / 53 * height);
	stocky.init("data/stocky.ttf", 1.0f / 53 * height * 2);
	roboto_modo_regular.init("data/RobotoMono-Bold.ttf", 1.0f / 53 * height * 1.5);
}

void Screen2D::display()
{
	pushScreenCoordinateMatrix();


	//carGauge->display();
	//freetype::display(digital_counter, 0.5 * width + 0.495 * height, 0.035 * height, "%i", carGauge->car->getCurrentGear());
	//freetype::display(digital_counter, 0.5 * width - 0.495 * height, 0.035 * height, "%i", static_cast<int>(carGauge->car->netTorque));
	//
	//freetype::display(stocky, 0.5 * width - 0.695 * height, 0.15 * height, "vLoc.x: %f", carGauge->car->vLoc.x);
	//freetype::display(stocky, 0.5 * width - 0.695 * height, 0.10 * height, "vLoc.y: %f", carGauge->car->vLoc.y);

	for (auto& testValue : testValues)
	{
		freetype::display(testValue.colorName, roboto_modo_regular, 0.5 * width + testValue.x * height / 100, testValue.y * height / 100, "%s", testValue.text.c_str());
	}
	testValues.clear();

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