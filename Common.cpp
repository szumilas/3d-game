#include "Common.h"

void Common::calculateFinalGeometry(TextureManager* textureManager)
{
	for (auto& point : points)
	{
		point.z = -0.08;
	}

	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::sett)].idTexture;
	}
}