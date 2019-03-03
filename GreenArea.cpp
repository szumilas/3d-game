#include "GreenArea.h"

void GreenArea::calculateFinalGeometry()
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = Game::textureManager.textures[static_cast<long>(Textures::grass)].idTexture;
	}
}