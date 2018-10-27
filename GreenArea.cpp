#include "GreenArea.h"

void GreenArea::display()
{
	glColor3f(0.0f, 0.4f, 0.1f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0.01f);
	}
	glEnd();
}