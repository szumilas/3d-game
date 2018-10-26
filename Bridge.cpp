#include "Bridge.h"

void Bridge::display()
{
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, -0.05f);
	}
	glEnd();
}