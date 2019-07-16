#include "GreenArea.h"

void GreenArea::calculateFinalGeometry()
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::grass)].idTexture;
	}
}