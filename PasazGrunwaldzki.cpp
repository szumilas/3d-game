#include "PasazGrunwaldzki.h"

PasazGrunwaldzki::PasazGrunwaldzki(MapObject& mapObject) : Building(mapObject)
{
	if (getId() == 101189502)
	{
		_color = Color::gray();
	}
	else
	{
		_color = Color{ 0.95f, 0.95f, 0.95f };
	}
}

void PasazGrunwaldzki::generateWalls()
{
	Building::generateWalls();


	if (getId() == 101195833)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::pasaz_grunwaldzki_single;
		}

		walls[36].textureName = Textures::pasaz_grunwaldzki_tunnel;
		walls[20].textureName = Textures::pasaz_grunwaldzki;
		walls[43].textureName = Textures::pasaz_grunwaldzki;
	}
	else if (getId() == 101212793)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::pasaz_grunwaldzki_single;
		}

		walls[4].textureName = Textures::pasaz_grunwaldzki_banner;
	}
	else if (getId() == 101213837)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::pasaz_grunwaldzki_front;
		}
	}
	else if (getId() == 101189502)
	{
		for (auto& wall : walls)
		{
			wall.textureName = Textures::no_texture;
		}
	}
}

void PasazGrunwaldzki::calculateFinalGeometry()
{
	Building::calculateFinalGeometry();

	if (getId() == 101213837)
	{
		for(int q = 21; q > 12; q--)
		{
			auto& wall = walls[q];
			
			Polygon newPolygon;

			Point secondOppositePoint{ wall.p2.x - 5.0f + 0.35f * (22 - q), wall.p2.y + 5.0f + 0.35f * (22 - q), _height + 9.5 };
			Point firstOppositePoint{ wall.p1.x - 5.0f + 0.35f * (23 - q), wall.p1.y + 5.0f + 0.35f * (23 - q), _height + 9.5 };

			if (q == 13)
			{
				firstOppositePoint.z += 0.49f;
				firstOppositePoint.x -= 0.6f;
			}

			newPolygon.points.push_back({ wall.p1.x, wall.p1.y, _height });
			newPolygon.points.push_back({ wall.p2.x, wall.p2.y, _height });
			newPolygon.points.push_back({ secondOppositePoint.x, secondOppositePoint.y, secondOppositePoint.z });
			newPolygon.points.push_back({ firstOppositePoint.x, firstOppositePoint.y, firstOppositePoint.z });

			newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
			newPolygon.texturePoints.push_back({ wall.xRatio, 0.0 });
			newPolygon.texturePoints.push_back({ wall.xRatio, wall.yRatio });
			newPolygon.texturePoints.push_back({ 0.0f, wall.yRatio });

			newPolygon.noOfPoints = newPolygon.texturePoints.size();
			newPolygon.idTexture = Game::textureManager.textures[static_cast<int>(Textures::glass_elevation)].idTexture;
			newPolygon.color = wall.color;

			auto newColor = wall.color.mixColor(selectedColor);
			newPolygon.additionalColor = wall.color.mixColor(newColor);

			polygons.push_back(newPolygon);
		}
	}
}