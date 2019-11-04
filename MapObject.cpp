#include "MapObject.h"
#include "PointInsidePolygonDetector.h"

void MapObject::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	for (auto& ref : refs)
	{
		Point newPoint;
		newPoint.x = nodes.at(ref).posX;
		newPoint.y = nodes.at(ref).posY;
		points.push_back(newPoint);
	}
}

void MapObject::calculateXYfromPoints(const std::vector<Point>& points)
{
	MapObject::points = points;
}

void MapObject::calculateFinalGeometry()
{

}

void MapObject::calculateBoundingCoordinates()
{
	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			if (point.x < minX)
				minX = point.x;
			if (point.x > maxX)
				maxX = point.x;
			if (point.y < minY)
				minY = point.y;
			if (point.y > maxY)
				maxY = point.y;
		}
	}
}

void MapObject::applyKnownValues()
{
	if (!height.empty()) { _height = std::stof(height); };
	if (!min_height.empty()) { _min_height = std::stof(min_height); };
	if (!width.empty()) { _width = std::stof(width); };
	if (!colour.empty())
	{
		if (colour[0] == '#')
		{
			_color.red = static_cast<float>(std::stoul(colour.substr(1, 2), nullptr, 16)) / 256;
			_color.green = static_cast<float>(std::stoul(colour.substr(3, 2), nullptr, 16)) / 256;
			_color.blue = static_cast<float>(std::stoul(colour.substr(5, 2), nullptr, 16)) / 256;
		}
		else
		{
			auto mapElement = colorMap.find(colour);
			if (mapElement != colorMap.end())
			{
				_color.red = std::get<0>(mapElement->second);
				_color.green = std::get<1>(mapElement->second);
				_color.blue = std::get<2>(mapElement->second);
			}
			else
			{
				std::string error = "unknown color!";
			}
		}
	};
}

void MapObject::select()
{
	isSelected = true;
	for (auto& polygon : polygons)
	{
		std::swap(polygon.color, polygon.additionalColor);
	}

	std::cout << id << " ";
}

void MapObject::deselect()
{
	isSelected = false;
	for (auto& polygon : polygons)
	{
		std::swap(polygon.color, polygon.additionalColor);
	}
}

void MapObject::optimizePoints()
{
	/*if (points.size() > 2)
	{
		for (auto it = points.begin() + 1; it + 2 != points.end();)
		{
			Line2D l1(*(it - 1), *(it + 1));

			if (l1.pointDistance(*(it)) < 0.1)
				it = points.erase(it);
			else
				it++;
		}
	}*/

	std::vector<Point> l1;
	std::vector<Point> l2;

	int it1 = 0;
	int it2 = 0;

	double longestDistance = 0.0;

	for (int q = 0; q < points.size() - 1; q++)
	{
		for (int w = 1; w < points.size() - 1; w++)
		{
			if (points[q].distance2D(points[w]) > longestDistance)
			{
				longestDistance = points[q].distance2D(points[w]);
				it1 = q;
				it2 = w;
			}
		}
	}

	for (int q = it1; ; q = (q + 1) % (points.size() - 1))
	{
		l1.push_back(points[q]);
		if (q == it2)
			break;
	}
	for (int q = it2; ; q = (q + 1) % (points.size() - 1))
	{
		l2.push_back(points[q]);
		if (q == it1)
			break;
	}



	auto optimizedPoints1 = DouglasPeuckerAlgorithm(l1, epsilon);
	auto optimizedPoints2 = DouglasPeuckerAlgorithm(l2, epsilon);

	points.clear();
	points.reserve(optimizedPoints1.size() + optimizedPoints2.size() - 1);
	points.insert(points.end(), optimizedPoints1.begin(), optimizedPoints1.end() - 1);
	points.insert(points.end(), optimizedPoints2.begin(), optimizedPoints2.end());
}

std::vector<Point> MapObject::DouglasPeuckerAlgorithm(std::vector<Point>& PointList, float epsilon)
{
	// Find the point with the maximum distance
	double dmax = 0.0;
	int index = 0;
	int end = PointList.size();
	for (int q = 1; q < end - 1; q++)
	{
		double d = Line2D(PointList[0], PointList.back()).pointDistance(PointList[q]);

		if (d > dmax)
		{
			index = q;
			dmax = d;
		}
	}

	std::vector<Point> res;

	// If max distance is greater than epsilon, recursively simplify
	if (dmax > epsilon)
	{
		// Recursive call
		std::vector<Point> l1(PointList.begin(), PointList.begin() + index + 1);
		std::vector<Point> l2(PointList.begin() + index, PointList.end());

		auto vec1 = DouglasPeuckerAlgorithm(l1, epsilon);
		auto vec2 = DouglasPeuckerAlgorithm(l2, epsilon);

		// Build the result list
		//ResultList[] = { recResults1[1...length(recResults1) - 1], recResults2[1...length(recResults2)] }
		res.reserve(vec1.size() + vec2.size() - 1); // preallocate memory
		res.insert(res.end(), vec1.begin(), vec1.end() - 1);
		res.insert(res.end(), vec2.begin(), vec2.end());
	}
	else
	{
		//ResultList[] = { PointList[1], PointList[end] }
		res.push_back(PointList[0]);
		res.push_back(PointList.back());
	}
	// Return the result
	return res;
}

void MapObject::shadeTheWall(Color& color, const vector2D& wallLine, float shadePower)
{
	vector2D equator(Point(0.0, 0.0), Point(1.0, 0.0));

	auto angle = vector2D::angle(equator, wallLine);

	if (angle <= PI / 2) // NORTH
	{
		auto ratio = angle / (PI / 2) * shadePower + (1 - shadePower);

		color.red *= ratio;
		color.green *= ratio;
		color.blue *= ratio;
	}
	else // SOUTH
	{
		auto ratio = (angle - PI / 2) / (PI / 2);

		color.red = ratio * (1 - color.red) * shadePower + color.red;
		color.green = ratio * (1 - color.green) * shadePower + color.green;
		color.blue = ratio * (1 - color.blue) * shadePower + color.blue;
	}
}

void MapObject::dividePointsPolygonIntoTriangles()
{
	if (points.size() > 2)
	{
		auto pointsCopy = points;
		points.pop_back();

		int firstPoint = 0;
		int secondPoint = 0;
		int thirdPoint = 0;

		int clockWiseCheckCount = 0;

		while (points.size() > 3)
		{
			if (clockWiseCheckCount > points.size() * 2)
			{
				points.clear();
				polygons.clear();
				for (int q = pointsCopy.size() - 2; q >= 0; q--)
				{
					points.push_back(pointsCopy[q]);
				}
				clockWiseCheckCount = 0;
				firstPoint = 0;
				pointsCopy = points;
			}

			secondPoint = (firstPoint + 1) % points.size();
			thirdPoint = (firstPoint + 2) % points.size();


			vector2D v1(points[firstPoint], points[secondPoint]);
			vector2D v2(points[firstPoint], points[thirdPoint]);

			if (vector2D::crossProduct(v1, v2) <= 0 && PointInsidePolygonDetector::countPointsInsidePolygon({ points[firstPoint], points[secondPoint], points[thirdPoint] }, points) <= 3)
			{
				Polygon polygon;
				polygon.points.push_back(points[firstPoint]);
				polygon.points.push_back(points[secondPoint]);
				polygon.points.push_back(points[thirdPoint]);

				polygon.texturePoints.push_back(points[firstPoint]);
				polygon.texturePoints.push_back(points[secondPoint]);
				polygon.texturePoints.push_back(points[thirdPoint]);

				polygon.noOfPoints = polygon.points.size();
				polygon.color = Color{ 1.0f, 1.0f, 1.0f };

				polygons.push_back(polygon);

				points.erase(points.begin() + secondPoint);
				firstPoint = firstPoint % points.size();
				clockWiseCheckCount = 0;
			}
			else
			{
				firstPoint = (firstPoint + 1) % points.size();
				clockWiseCheckCount++;
			}
		}

		Polygon polygon;
		polygon.points.push_back(points[0]);
		polygon.points.push_back(points[1]);
		polygon.points.push_back(points[2]);

		polygon.texturePoints.push_back(points[0]);
		polygon.texturePoints.push_back(points[1]);
		polygon.texturePoints.push_back(points[2]);

		polygon.noOfPoints = polygon.points.size();
		polygon.color = Color{ 1.0f, 1.0f, 1.0f };

		polygons.push_back(polygon);
	}
}

void MapObject::createBothRailsForSymmetryAxis(const std::vector<Point>& axisPoints, std::vector<Point>& finalLeftRail, std::vector<Point>& finalRightRail, float gauge)
{
	std::vector<Point> leftRail;
	std::vector<Point> rightRail;

	for (size_t q = 0, limit = axisPoints.size() - 1; q < limit; q++)
	{
		const Point& firstPoint = axisPoints[q];
		const Point& secondPoint = axisPoints[q + 1];

		float distance = firstPoint.distance2D(secondPoint);

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

	finalLeftRail.push_back(leftRail[0]);
	finalRightRail.push_back(rightRail[0]);

	for (size_t q = 1, limit = leftRail.size() - 2; q < limit; q += 2)
	{
		Line2D leftFirst(leftRail[q - 1], leftRail[q]);
		Line2D leftSecond(leftRail[q + 1], leftRail[q + 2]);
		auto intersectionPointLeft = leftFirst.calcuateIntersectionPoint(leftSecond);

		finalLeftRail.push_back(intersectionPointLeft);

		Line2D rightFirst(rightRail[q - 1], rightRail[q]);
		Line2D rightSecond(rightRail[q + 1], rightRail[q + 2]);
		auto intersectionPointRight = rightFirst.calcuateIntersectionPoint(rightSecond);

		finalRightRail.push_back(intersectionPointRight);
	}

	finalLeftRail.push_back(leftRail[leftRail.size() - 1]);
	finalRightRail.push_back(rightRail[rightRail.size() - 1]);

}

Color MapObject::getUniqueColorBasedOnId()
{
	if (uniqueColorBasedOnId.red == -1)
	{
		Color newColor;
		int a = id % 10;
		int b = (id / 10) % 10;
		int c = (id / 100) % 10;

		float color1 = 0.01f * ((a * 10 + b + c) % 100);
		float color2 = 0.01f * ((b * 10 + a + c) % 100);
		float color3 = 0.01f * (((a * 100 + 10 * b) / 10) % 100);

		if (a % 3 == 0)
			newColor = Color(color1, color2, color3);
		else if(a % 3 == 1)
			newColor = Color(color3, color1, color2);
		else
			newColor = Color(color2, color3, color1);

		uniqueColorBasedOnId = newColor;
	}

	return uniqueColorBasedOnId;
}