#include "Building.h"

void Building::display()
{
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, 15);
	}
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	for (size_t q = 0; q < points.size() - 1; q++)
	{
		auto point = points[q];
		auto nextPoint = points[q + 1];

		glBegin(GL_POLYGON);
		glVertex3f(point.x, point.y, 0);
		glVertex3f(nextPoint.x, nextPoint.y, 0);
		glVertex3f(nextPoint.x, nextPoint.y, 15);
		glVertex3f(point.x, point.y, 15);
		glEnd();
	}
}