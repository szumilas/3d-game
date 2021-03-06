#include "Common.h"

Common::Common(MapObject& mapObject) : MapObject(mapObject)
{
	drawingPriority = DrawingPriority::whenClose;
}

void Common::calculateFinalGeometry()
{
	for (auto& point : points)
	{
		point.z = -0.08;
	}

	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::sett)].idTexture;
	}
}