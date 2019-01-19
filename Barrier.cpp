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

	if (barrier == "hedge")
		_height *= 0.9;

	if (sidesOfBarrier.size() == 2)
	{
		std::vector<Point>& leftSide = sidesOfBarrier[0];
		std::vector<Point>& rightSide = sidesOfBarrier[1];

		for (size_t q = 0; q < leftSide.size() - 1; q++)
		{
			Polygon polygon;
			polygon.points.push_back({ leftSide[q].x, leftSide[q].y, _height });
			polygon.points.push_back({ leftSide[q + 1].x, leftSide[q + 1].y, _height });
			polygon.points.push_back({ rightSide[q + 1].x, rightSide[q + 1].y, _height });
			polygon.points.push_back({ rightSide[q].x, rightSide[q].y, _height });

			float xRatio = static_cast<int>(leftSide[q].distance2D(leftSide[q + 1]) / textureManager->textures[static_cast<long>(textureName)].realWidth);
			float yRatio = 1;

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio)
				yRatio = 1.0f;

			if (barrier == "hedge")
				yRatio *= 0.8;

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