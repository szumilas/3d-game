#include "Building.h"

Building::Building(MapObject& mapObject) : MapObject(mapObject)
{

	if (building == "roof" || building_part == "roof")
	{
		if (!_height)
		{
			_height = 6.5f;
		}
		_min_height = std::min(6.0f, _height - 0.5f);
	}
	if (!_height)
	{
		_height = 15.0f;
	}
	if (colour.empty())
	{
		_red = 0.5f;
		_green = 0.5f;
		_blue = 0.5f;
	}
};

void Building::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	generateWalls();
}

void Building::generateWalls()
{
	vector2D equator(Point(0.0, 0.0), Point(1.0, 0.0));

	for (int q = 0; q < static_cast<int>(points.size()) - 1; q++)
	{
		Wall newWall;
		newWall.p1 = points[q];
		newWall.p2 = points[q + 1];

		newWall.color.red = _red;
		newWall.color.green = _green;
		newWall.color.blue = _blue;

		vector2D wallLine(newWall.p1, newWall.p2);
		shadeTheWall(newWall.color, wallLine, 0.5f);

		walls.push_back(newWall);
	}
}

void Building::display()
{
	for (auto& wall : walls)
	{
		glBegin(GL_POLYGON);
		if (isSelected)
		{
			auto newColor = wall.color.mixColor(selectedColor);
			glColor3f(newColor.red, newColor.green, newColor.blue);
		}
		else
			glColor3f(wall.color.red, wall.color.green, wall.color.blue);

		glVertex3f(wall.p1.x, wall.p1.y, _min_height);
		glVertex3f(wall.p2.x, wall.p2.y, _min_height);
		glVertex3f(wall.p2.x, wall.p2.y, _height);
		glVertex3f(wall.p1.x, wall.p1.y, _height);
		glEnd();
	}

	/*glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_POLYGON);
	for (auto& point : points)
	{
		glVertex3f(point.x, point.y, _height);
	}
	glEnd();*/

	/*
	glLineWidth(3.0f);
	for (int q = 0; q < static_cast<int>(points.size()) - 1; q++)
	{
		auto point = points[q];
		auto nextPoint = points[q + 1];

		glBegin(GL_POLYGON);
		glColor3f(_red, _green, _blue);
		glVertex3f(point.x, point.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _height);
		glVertex3f(point.x, point.y, _height);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(point.x, point.y, _min_height);
		glVertex3f(point.x, point.y, _height);
		glEnd();
	}

	glLineWidth(1.0f);
	*/


	/*glLineWidth(2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(points[0].x, points[0].y, 0.0f);
	glVertex3f(points[0].x, points[0].y, 20.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(points[1].x, points[1].y, 0.0f);
	glVertex3f(points[1].x, points[1].y, 20.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(points[2].x, points[2].y, 0.0f);
	glVertex3f(points[2].x, points[2].y, 20.0f);
	glEnd();*/
	

}