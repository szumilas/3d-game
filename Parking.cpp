#include "Parking.h"

void Parking::display()
{
	glColor3f(0.25f, 0.25f, 0.25f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0.0f);
	}
	glEnd();
}