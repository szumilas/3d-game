#include "Barrier.h"

void Barrier::calculateFinalGeometry(TextureManager* textureManager)
{
	std::vector<std::vector<Point>> sidesOfBarrier;

	if (_width)
	{
		std::vector<Point> leftSide;
		std::vector<Point> rightSide;

		createBothRailsForSymmetryAxis(points, leftSide, rightSide, _width);

		sidesOfBarrier.push_back(leftSide);
		sidesOfBarrier.push_back(rightSide);
	}
	else
	{
		sidesOfBarrier.push_back(points);
	}

	for (auto& pointsToDraw : sidesOfBarrier)
	{
		for (size_t q = 0; q < pointsToDraw.size() - 1; q++)
		{
			auto& point = pointsToDraw[q];
			auto& nextPoint = pointsToDraw[q + 1];

			Polygon polygon;
			polygon.points.push_back({ point.x, point.y, 0 });
			polygon.points.push_back({ nextPoint.x, nextPoint.y, 0 });
			polygon.points.push_back({ nextPoint.x, nextPoint.y, _height });
			polygon.points.push_back({ point.x, point.y, _height });

			float xRatio = static_cast<int>(point.distance2D(nextPoint) / textureManager->textures[static_cast<long>(textureName)].realWidth);
			float yRatio = static_cast<int>(_height) / textureManager->textures[static_cast<long>(textureName)].realHeight;

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio)
				yRatio = 1.0f;

			if (yRatio == 1.0f)
				yRatio = 0.95f;

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ xRatio, 0 });
			polygon.texturePoints.push_back({ xRatio, yRatio });
			polygon.texturePoints.push_back({ 0, yRatio });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = Color{ 1.0f, 1.0f, 1.0f };

			polygon.idTexture = textureManager->textures[static_cast<unsigned int>(textureName)].idTexture;

			polygons.push_back(polygon);

		}
	}
}