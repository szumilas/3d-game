#include "Street.h"
#include "PointInsidePolygonDetector.h"

void Street::calculateFinalGeometry(TextureManager* textureManager)
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

				polygon.idTexture = textureManager->textures[static_cast<long>(Textures::asphalt)].idTexture;

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

		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::asphalt)].idTexture;

		polygons.push_back(polygon);
	}
}