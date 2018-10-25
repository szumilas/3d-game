#include "Street.h"

void Street::display()
{
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0);
	}
	glEnd();
}