#include "HighlightedObject.h"

void HighlightedObject::display()
{
	glColor3f(0.5f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 0.0f);
	}
	glEnd();
}