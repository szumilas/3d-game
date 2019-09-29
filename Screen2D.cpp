#include "Screen2D.h"

std::unique_ptr<Screen2D> Screen2D::_instance;

Screen2D::Screen2D(int width, int height)
{
	setSize(width, height);
	loadFonts();
}

Screen2D::~Screen2D()
{
	digital_counter.clean();
	stocky.clean();
	roboto_modo_regular.clean();
}

void Screen2D::Init(int width, int height)
{
	_instance = std::unique_ptr<Screen2D>(new Screen2D(width, height));
}

std::unique_ptr<Screen2D>& Screen2D::Instance()
{
	return _instance;
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
	squada_one_regular.init("data/SquadaOne-Regular.ttf", 1.0f / 53 * height * 1.0);
	squada_one_regular_big.init("data/SquadaOne-Regular.ttf", 1.0f / 53 * height * 1.5);
}

void Screen2D::display()
{
	for (auto& textValue : textValues)
	{
		freetype::display(textValue.colorName, *textValue.font, 0.5 * width + textValue.x / 100 * height, textValue.y / 100 * height, "%s", textValue.text.c_str());
	}
	textValues.clear();
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