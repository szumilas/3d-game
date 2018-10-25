#include "GreenArea.h"

void GreenArea::display()
{
	glColor3f(0, 0.4, 0.1);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0.01);
	}
	glEnd();
}