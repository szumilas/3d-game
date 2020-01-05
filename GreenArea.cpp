#include "GreenArea.h"

GreenArea::GreenArea(MapObject& mapObject) : MapObject(mapObject)
{
	if (height.empty())
		_height = -0.05;

	drawingPriority = DrawingPriority::whenClose;
}

void GreenArea::calculateFinalGeometry()
{
	std::for_each(points.begin(), points.end(), [&](auto& p) { p.z = _height; });

	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::grass)].idTexture;
	}
}