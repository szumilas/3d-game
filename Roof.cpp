#include "Roof.h"

#include "PointInsidePolygonDetector.h"




float measureAngle(Point& currentPoint, Point& nextPoint, Point& previousPoint)
{
	float lenA = currentPoint.distance(nextPoint);
	float lenB = currentPoint.distance(previousPoint);
	float lenC = nextPoint.distance(previousPoint);

	return acos((lenA * lenA + lenB * lenB - lenC * lenC) / (2 * lenA * lenB));
}



Roof::Roof(MapObject& mapObject) : MapObject(mapObject)
{
	if (!_height)
	{
		_roof_level = 15.0f;
	}
	_red = 1.0f;

};

void Roof::display()
{
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(_red, _green, _blue);
	for (size_t q = 0; q < points.size() - 1; q++)
	{
		//auto point = points[q];
		//auto nextPoint = points[q + 1];
		
		glVertex3f(points[q].x, points[q].y, _roof_level);
		
	}
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(2.0f);
	glColor3f(0, 1.0f, 0);

	glBegin(GL_LINES);
	glVertex3f(points[0].x, points[0].y, _roof_level);
	glVertex3f(points[0].x, points[0].y, _roof_level + 5);
	glEnd();


	for (size_t q = 0, limit = specialPoints.size() - 1; q < limit; q+=2)
	{
		glBegin(GL_LINES);
		glVertex3f(specialPoints[q].x, specialPoints[q].y, specialPoints[q].z);
		glVertex3f(specialPoints[q + 1].x, specialPoints[q + 1].y, specialPoints[q + 1].z);
		glEnd();
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	for (size_t q = 0, limit = roofEdges.size() - 1; q < limit; q += 2)
	{
		glBegin(GL_LINES);
		glVertex3f(roofEdges[q].x, roofEdges[q].y, _roof_level);
		glVertex3f(roofEdges[q + 1].x, roofEdges[q + 1].y, roofEdges[q + 1].z);
		glEnd();
	}

	
	glLineWidth(1.0f);

}

void Roof::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	float gamma = 3.14 / 6;

	std::vector<std::tuple<Point, Point, float>> roofData;

	for(size_t q = 0; q < points.size() - 1; q++)
	{
		Point& currentPoint = points[q];

		Point& nextPoint = points[q + 1];
		Point& previousPoint = points[(q - 1) % (points.size())];

		float alpha = measureAngle(currentPoint, previousPoint, nextPoint) / 2;
		float beta = atan(tan(gamma) * sin(alpha));

		Line2D firstLine(currentPoint, nextPoint);
		Line2D secondLine(currentPoint, previousPoint);

		Line2D firstBisector;
		Line2D secondBisector;

		firstBisector.calculateBisector(firstLine, secondLine);
		secondBisector.calculateBisector(firstLine, secondLine, true);

		/*
		float a = firstLine.A;
		float b = firstLine.B;
		float c = firstLine.C;
		float d = secondLine.A;
		float e = secondLine.B;
		float f = secondLine.C;
		float g = sqrt(pow(a, 2) + pow(b, 2)) / sqrt(pow(d, 2) + pow(e, 2));

		Line2D firstBisector(a + d * g, b + e * g, c + f * g);
		Line2D secondBisector(a - d * g, b - e * g, c - f * g);
		*/

		//specialPoints.push_back({ 1000, (-firstLine.A * 1000 - firstLine.C) / firstLine.B, _roof_level });
		//specialPoints.push_back({ -1000, (-firstLine.A * (-1000) - firstLine.C) / firstLine.B, _roof_level });
		//
		//specialPoints.push_back({ 1000, (-secondLine.A * 1000 - secondLine.C) / secondLine.B, _roof_level });
		//specialPoints.push_back({ -1000, (-secondLine.A * (-1000) - secondLine.C) / secondLine.B, _roof_level });

		//specialPoints.push_back({ 1000, (-firstBisector.A * 1000 - firstBisector.C) / firstBisector.B, _roof_level });
		//specialPoints.push_back({ -1000, (-firstBisector.A * (-1000) - firstBisector.C) / firstBisector.B, _roof_level });
		//
		//specialPoints.push_back({ 1000, (-secondBisector.A * 1000 - secondBisector.C) / secondBisector.B, _roof_level });
		//specialPoints.push_back({ -1000, (-secondBisector.A * (-1000) - secondBisector.C) / secondBisector.B, _roof_level });


		float r = 0.05f;

		std::vector<Point> pointsToCheck;

		//get any point on bisector

		for (auto& line : { firstBisector , secondBisector })
		{
			float x;
			float y;
			if (firstBisector.B != 0)
			{
				x = 1.0f;
				y = (-line.A - line.C) / line.B;
			}
			else
			{
				x = -line.C / line.A;
				y = 1;
			}

			Point vec{ x - currentPoint.x, y - currentPoint.y };
			float distance = vec.distance(Point{ 0.0f, 0.0f });

			x = currentPoint.x + vec.x * r / distance;
			y = currentPoint.y + vec.y * r / distance;

			pointsToCheck.push_back({ x, y, _roof_level });
			//pointsToCheck.push_back({ x, y, _roof_level + 1.0f });

			x = currentPoint.x + vec.x * (-r) / distance;
			y = currentPoint.y + vec.y * (-r) / distance;

			pointsToCheck.push_back({ x, y, _roof_level });
			//pointsToCheck.push_back({ x, y, _roof_level + 1.0f });
		}

		std::swap(pointsToCheck[1], pointsToCheck[2]);


		PointInsidePolygonDetector pointInsidePolygonDetector;

		bool pInside[4];
		pInside[0] = pointInsidePolygonDetector.isInside(points, pointsToCheck[0]);
		pInside[1] = pointInsidePolygonDetector.isInside(points, pointsToCheck[1]);
		pInside[2] = pointInsidePolygonDetector.isInside(points, pointsToCheck[2]);
		pInside[3] = pointInsidePolygonDetector.isInside(points, pointsToCheck[3]);

		for (int w = 0; w < 4; w++)
		{
			if (pInside[w] == true && pInside[(w + 2) % 4] == false)
			{
				specialPoints.push_back({ pointsToCheck[w].x, pointsToCheck[w].y, _roof_level });
				specialPoints.push_back({ pointsToCheck[w].x, pointsToCheck[w].y, _roof_level + 1 });

				roofData.push_back(std::make_tuple(currentPoint, pointsToCheck[w], beta));
				break;
			}
		}
	}

	float H = 0.01f;
	bool found = false;
	for (;H < 25.0f; H += 0.001f)
	{
		for (auto& data : roofData)
		{
			auto& firstPoint = std::get<0>(data);
			auto& secondPoint = std::get<1>(data);
			auto& beta = std::get<2>(data);

			float distance2d = firstPoint.distance(secondPoint);

			float newDist = H / tan(beta);

			Point newPoint;

			newPoint.x = firstPoint.x + (secondPoint.x - firstPoint.x) * newDist / distance2d;
			newPoint.y = firstPoint.y + (secondPoint.y - firstPoint.y) * newDist / distance2d;
			newPoint.z = _roof_level + H;

			roofEdges.push_back(firstPoint);
			roofEdges.push_back(newPoint);
		}

		for (size_t q = 1, limit = roofEdges.size() - 2; q < limit; q += 2)
		{
			if (roofEdges[q].distance(roofEdges[q + 2]) < 0.02f)
			{
				found = true;
				break;
			}
		}

		if (found)
			break;
		else
			roofEdges.clear();
	}

	int hhh = 5;
}