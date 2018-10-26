#include "Barrier.h"

void Barrier::display()
{
	glColor3f(0.2, 0.2, 0.4);
	for (size_t q = 0; q < points.size() - 1; q++)
	{
		auto point = points[q];
		auto nextPoint = points[q + 1];

		glBegin(GL_POLYGON);
		glVertex3f(point.x, point.y, 0);
		glVertex3f(nextPoint.x, nextPoint.y, 0);
		glVertex3f(nextPoint.x, nextPoint.y, 1.5);
		glVertex3f(point.x, point.y, 1.5);
		glEnd();
	}
}