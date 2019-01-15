#include "Footway.h"

void Footway::calculateFinalGeometry(TextureManager* textureManager)
{
	for (size_t q = 0, limit = finalLeftRail.size() - 1; q < limit; q++)
	{
		Polygon newPolygon;

		newPolygon.points.push_back({ finalLeftRail[q].x, finalLeftRail[q].y, _min_height });
		newPolygon.points.push_back({ finalLeftRail[q + 1].x, finalLeftRail[q + 1].y, _min_height });
		newPolygon.points.push_back({ finalRightRail[q + 1].x, finalRightRail[q + 1].y, _min_height });
		newPolygon.points.push_back({ finalRightRail[q].x, finalRightRail[q].y, _min_height });

		newPolygon.texturePoints.push_back({ finalLeftRail[q].x, finalLeftRail[q].y, _min_height });
		newPolygon.texturePoints.push_back({ finalLeftRail[q + 1].x, finalLeftRail[q + 1].y, _min_height });
		newPolygon.texturePoints.push_back({ finalRightRail[q + 1].x, finalRightRail[q + 1].y, _min_height });
		newPolygon.texturePoints.push_back({ finalRightRail[q].x, finalRightRail[q].y, _min_height });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = textureManager->textures[static_cast<unsigned int>(Textures::paving)].idTexture;
		newPolygon.color = Color{ 1.0f, 1.0f, 1.0f };

		newPolygon.additionalColor = newPolygon.color.mixColor(selectedColor);

		polygons.push_back(newPolygon);
	}
}

void Footway::calculateXYfromRef(const std::map<long long, node> &nodes)
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