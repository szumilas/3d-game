#include "Street.h"

Street::Street(MapObject& mapObject) : MapObject(mapObject)
{
	drawingPriority = DrawingPriority::whenClose;
}

void Street::calculateFinalGeometry()
{
	std::for_each(points.begin(), points.end(), [&](auto& p) { p.z = _height; });

	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::asphalt)].idTexture;

		if (area_highway == "cycleway")
		{
			polygon.color.red = 0.9f;
			polygon.color.green = 0.6f;
			polygon.color.blue = 0.6f;
		}
		else if (area_highway == "footway")
		{
			polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::paving)].idTexture;
		}
		else  if (area_highway == "service")
		{
			polygon.color.red = 0.9;
			polygon.color.green = 0.9;
			polygon.color.blue = 0.9;
		}

	}
}