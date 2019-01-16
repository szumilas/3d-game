#include "Street.h"

void Street::calculateFinalGeometry(TextureManager* textureManager)
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::asphalt)].idTexture;
	}
}