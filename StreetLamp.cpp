#include "StreetLamp.h"

StreetLamp::StreetLamp(long long ref)
{
	refs.push_back(ref);
	_height = 4.0f;
}

void StreetLamp::display()
{
	glColor3f(0.6f, 0.6f, 0.6f);

	for (size_t q = 0; q < points.size() - 1; q++)
	{
		Point& point = points[q];
		Point& nextPoint = points[q + 1];

		glBegin(GL_POLYGON);
		glVertex3f(point.x, point.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _height);
		glVertex3f(point.x, point.y, _height);
		glEnd();
	}
}

void StreetLamp::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	Point newPoint;
	float size = 0.15;

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

}