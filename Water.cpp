#include "Water.h"

void Water::calculateFinalGeometry(TextureManager* textureManager)
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::water)].idTexture;
	}
}