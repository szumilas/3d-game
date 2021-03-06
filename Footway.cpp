#include "Footway.h"

Footway::Footway()
{
	_min_height = 0.0f;
	drawingPriority = DrawingPriority::whenClose;
}

Footway::Footway(MapObject& mapObject) : MapObject(mapObject)
{
	_min_height = -0.025f;
	_color.red = 1.0f;
	_color.green = 1.0f;
	_color.blue = 1.0f;
	textureName = Textures::paving;
	drawingPriority = DrawingPriority::whenClose;
};

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
		newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;
		newPolygon.color = Color{ _color.red, _color.green, _color.blue };

		newPolygon.additionalColor = newPolygon.color.mixColor(selectedColor);

		polygons.push_back(newPolygon);
	}

	points.clear();

	for (auto& point : finalLeftRail)
	{
		points.push_back(point);
	}

	for (int q = finalRightRail.size() - 1; q >= 0; --q)
	{
		points.push_back(finalRightRail[q]);
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