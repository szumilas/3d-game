#include "Water.h"

void Water::calculateFinalGeometry()
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::water)].idTexture;
	}
}