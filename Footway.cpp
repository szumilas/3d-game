#include "Footway.h"

void Footway::calculateFinalGeometry()
{
	for (size_t q = 0, limit = finalLeftRail.size() - 1; q < limit; q++)
	{
		Polygon newPolygon;

		newPolygon.points.push_back({ finalLeftRail[q].x, finalLeftRail[q].y, _min_height });
		newPolygon.points.push_back({ finalLeftRail[q + 1].x, finalLeftRail[q + 1].y, _min_height });
		newPolygon.points.push_back({ finalRightRail[q + 1].x, finalRightRail[q + 1].y, _min_height });
		newPolygon.points.push_back({ finalRightRail[q].x, finalRightRail[q].y, _min_height });

		newPolygon.texturePoints.push_back({ finalLeftRail[q].x, finalLeftRail[q].y });
		newPolygon.texturePoints.push_back({ finalLeftRail[q + 1].x, finalLeftRail[q + 1].y });
		newPolygon.texturePoints.push_back({ finalRightRail[q + 1].x, finalRightRail[q + 1].y });
		newPolygon.texturePoints.push_back({ finalRightRail[q].x, finalRightRail[q].y });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = Game::textureManager.textures[static_cast<unsigned int>(textureName)].idTexture;
		newPolygon.color = Color{ _color.red, _color.green, _color.blue };

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

	points = DouglasPeuckerAlgorithm(points, 0.1f);

	createBothRailsForSymmetryAxis(points, finalLeftRail, finalRightRail, gauge);

}