#include "HighlightedObject.h"

void HighlightedObject::display()
{
	glColor3f(0.5, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0);
	}
	glEnd();
}