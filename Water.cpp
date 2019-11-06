#include "Water.h"

Water::Water(MapObject& mapObject) : MapObject(mapObject)
{
	for (auto& point : points)
	{
		point.z = _min_height;
	}
}

void Water::calculateFinalGeometry()
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::water)].idTexture;
	}
}