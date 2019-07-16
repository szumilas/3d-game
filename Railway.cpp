#include "Railway.h"

void Railway::calculateFinalGeometry()
{
	struct Rail
	{
		std::vector<Point> leftSide;
		std::vector<Point> rightSide;
	};

	Rail left;
	Rail right;

	createBothRailsForSymmetryAxis(finalLeftRail, left.rightSide, left.leftSide, 0.1f);
	createBothRailsForSymmetryAxis(finalRightRail, right.leftSide, right.rightSide, 0.1f);

	for (auto& rail : { &left, &right })
	{
		for (size_t q = 0, limit = rail->leftSide.size() - 1; q < limit; q++)
		{
			Polygon newPolygon;

			newPolygon.points.push_back({ rail->leftSide[q].x, rail->leftSide[q].y, _min_height });
			newPolygon.points.push_back({ rail->leftSide[q + 1].x, rail->leftSide[q + 1].y, _min_height });
			newPolygon.points.push_back({ rail->rightSide[q + 1].x, rail->rightSide[q + 1].y, _min_height });
			newPolygon.points.push_back({ rail->rightSide[q].x, rail->rightSide[q].y, _min_height });

			newPolygon.texturePoints.push_back({ 0, 0 });
			newPolygon.texturePoints.push_back({ 1, 0 });
			newPolygon.texturePoints.push_back({ 1, 1 });
			newPolygon.texturePoints.push_back({ 0, 1 });

			newPolygon.noOfPoints = newPolygon.texturePoints.size();
			newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::rail)].idTexture;
			newPolygon.color = Color{ 1.0f, 1.0f, 1.0f };

			newPolygon.additionalColor = newPolygon.color.mixColor(selectedColor);

			polygons.push_back(newPolygon);
		}
	}
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

	createBothRailsForSymmetryAxis(points, finalLeftRail, finalRightRail, gauge);

}