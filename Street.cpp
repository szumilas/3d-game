#include "Street.h"

void Street::display()
{
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0.0f);
	}
	glEnd();
}