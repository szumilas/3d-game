#include "GreenArea.h"

void GreenArea::calculateFinalGeometry(TextureManager* textureManager)
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::grass)].idTexture;
	}
}