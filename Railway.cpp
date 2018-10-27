#include "Railway.h"

void Railway::display()
{
	glColor3f(0.7f, 0.7f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (size_t limit = points.size() * 2 / 3, q = points.size() / 3; q < limit; q++)
	{
		glVertex3f(points[q].x, points[q].y, 0.05f);
	}
	glEnd();

	glColor3f(0.7f, 0.7f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (size_t limit = points.size(), q = points.size() * 2 / 3; q < limit; q++)
	{
		glVertex3f(points[q].x, points[q].y, 0.05f);
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (size_t limit = points.size() / 3, q = 0; q < limit; q++)
	{
		glVertex3f(points[q].x, points[q].y, 5.0f);
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (size_t limit = points.size() / 3, q = 0; q < limit; q++)
	{
		glVertex3f(points[q].x, points[q].y, 5.2f);
	}
	glEnd();
}

void Railway::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	for (auto& ref : refs)
	{
		Point newPoint;
		newPoint.x = nodes.at(ref).posX;
		newPoint.y = nodes.at(ref).posY;
		points.push_back(newPoint);
	}

	std::vector<Point> leftRail;
	std::vector<Point> rightRail;

	for (size_t q = 0, limit = points.size() - 1; q < limit; q++)
	{
		Point& firstPoint = points[q];
		Point& secondPoint = points[q + 1];

		float distance = firstPoint.distance(secondPoint);
		
		//first point
		{
			Point pointToRotate{ firstPoint.x + gauge / 2 / distance * (secondPoint.x - firstPoint.x), firstPoint.y + gauge / 2 / distance * (secondPoint.y - firstPoint.y) };
			Point rotatedPoint;

			float alpha = 3.14 / 2;
			rotatedPoint.x = firstPoint.x + (pointToRotate.x - firstPoint.x) * cos(alpha) - (pointToRotate.y - firstPoint.y) * sin(alpha);
			rotatedPoint.y = firstPoint.y + (pointToRotate.x - firstPoint.x) * sin(alpha) + (pointToRotate.y - firstPoint.y) * cos(alpha);
			leftRail.push_back(rotatedPoint);
		}
		{
			Point pointToRotate{ firstPoint.x + gauge / 2 / distance * (secondPoint.x - firstPoint.x), firstPoint.y + gauge / 2 / distance * (secondPoint.y - firstPoint.y) };
			Point rotatedPoint;

			float alpha = -3.14 / 2;
			rotatedPoint.x = firstPoint.x + (pointToRotate.x - firstPoint.x) * cos(alpha) - (pointToRotate.y - firstPoint.y) * sin(alpha);
			rotatedPoint.y = firstPoint.y + (pointToRotate.x - firstPoint.x) * sin(alpha) + (pointToRotate.y - firstPoint.y) * cos(alpha);
			rightRail.push_back(rotatedPoint);
		}


		//second point
		{
			Point pointToRotate{ secondPoint.x + gauge / 2 / distance * (firstPoint.x - secondPoint.x), secondPoint.y + gauge / 2 / distance * (firstPoint.y - secondPoint.y) };
			Point rotatedPoint;

			float alpha = 3.14 / 2;
			rotatedPoint.x = secondPoint.x + (pointToRotate.x - secondPoint.x) * cos(alpha) - (pointToRotate.y - secondPoint.y) * sin(alpha);
			rotatedPoint.y = secondPoint.y + (pointToRotate.x - secondPoint.x) * sin(alpha) + (pointToRotate.y - secondPoint.y) * cos(alpha);
			rightRail.push_back(rotatedPoint);
		}
		{
			Point pointToRotate{ secondPoint.x + gauge / 2 / distance * (firstPoint.x - secondPoint.x), secondPoint.y + gauge / 2 / distance * (firstPoint.y - secondPoint.y) };
			Point rotatedPoint;

			float alpha = -3.14 / 2;
			rotatedPoint.x = secondPoint.x + (pointToRotate.x - secondPoint.x) * cos(alpha) - (pointToRotate.y - secondPoint.y) * sin(alpha);
			rotatedPoint.y = secondPoint.y + (pointToRotate.x - secondPoint.x) * sin(alpha) + (pointToRotate.y - secondPoint.y) * cos(alpha);
			leftRail.push_back(rotatedPoint);
		}
	}

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

	finalLeftRail.push_back(leftRail[0]);
	finalRightRail.push_back(rightRail[0]);

	for (size_t q = 1, limit = leftRail.size() - 1; q < limit; q+=2)
	{
		finalLeftRail.push_back(Point{ (leftRail[q].x + leftRail[q + 1].x) / 2, (leftRail[q].y + leftRail[q + 1].y) / 2 });
		finalRightRail.push_back(Point{ (rightRail[q].x + rightRail[q + 1].x) / 2, (rightRail[q].y + rightRail[q + 1].y) / 2 });
	}

	finalLeftRail.push_back(leftRail[leftRail.size() - 1]);
	finalRightRail.push_back(rightRail[rightRail.size() - 1]);


	points.reserve(points.size() + finalLeftRail.size());
	points.insert(points.end(), finalLeftRail.begin(), finalLeftRail.end());


	points.reserve(points.size() + finalRightRail.size());
	points.insert(points.end(), finalRightRail.begin(), finalRightRail.end());
}