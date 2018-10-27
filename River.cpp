#include "River.h"

void River::display()
{
	glColor3f(0.2f, 0.2f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, -5.0f);
	}
	glEnd();
}