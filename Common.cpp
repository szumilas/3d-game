#include "Common.h"

void Common::display()
{
	glColor3f(0.8f, 0.5f, 0.0f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, -0.05f);
	}
	glEnd();
}