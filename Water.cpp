#include "Water.h"

void Water::calculateFinalGeometry()
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = Game::textureManager.textures[static_cast<long>(Textures::water)].idTexture;
	}
}