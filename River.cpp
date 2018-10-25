#include "River.h"

void River::display()
{
	glColor3f(0.2, 0.2, 1.0);
	glBegin(GL_LINE_STRIP);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, -5);
	}
	glEnd();
}