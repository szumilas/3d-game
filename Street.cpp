#include "Street.h"

void Street::calculateFinalGeometry(TextureManager* textureManager)
{
	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.idTexture = textureManager->textures[static_cast<long>(Textures::asphalt)].idTexture;

		if (area_highway == "cycleway")
		{
			polygon.color.red = 0.9f;
			polygon.color.green = 0.6f;
			polygon.color.blue = 0.6f;
		}
		else if (area_highway == "footway")
		{
			polygon.idTexture = textureManager->textures[static_cast<long>(Textures::paving)].idTexture;
		}
		else  if (area_highway == "service")
		{
			polygon.color.red = 0.9;
			polygon.color.green = 0.9;
			polygon.color.blue = 0.9;
		}

	}
}