#include "GrunwaldzkiCenter.h"

GrunwaldzkiCenter::GrunwaldzkiCenter(MapObject& mapObject) : Building(mapObject)
{
	_color = Color::lightGray();

	if (getId() == 101195194 || getId() == 1442935)
	{
		_min_height = 6.0f;
		_height = 18.0f;
	}
	else if (getId() == 101212152 || getId() == 101215218)
	{
		_min_height = 6.0f;
		_height = 45.0f;
	}
	else if (getId() == 101188144)
	{
		_height = 45.0f;
	}
	else if (getId() == 101207075)
	{
		_min_height = 6.0f;
		_height = 7.0f;
		_color = Color{0.41f, 0.41f, 0.37f};
	}
	else if (getId() == 101191558)
	{
		_height = 18.0f;
	}
}

void GrunwaldzkiCenter::generateWalls()
{
	Building::generateWalls();

	for (auto& wall : walls)
	{
		if (getId() != 101207075)
		{
			wall.textureName = Textures::grunwaldzki;
		}
		else
		{
			wall.textureName = Textures::no_texture;
		}
	}
}

void GrunwaldzkiCenter::calculateFinalGeometry(TextureManager* textureManager)
{
	Building::calculateFinalGeometry(textureManager);

	if (getId() == 101207075)
	{
		for (int q = 6; q >= 0; q--)
		{
			auto& wall = walls[q];

			Polygon newPolygon;

			Point secondOppositePoint{ wall.p2.x + 5.0f + 0.35f * (12 - q), wall.p2.y - 5.0f + 0.35f * (12 - q), _height + 9.5 };
			Point firstOppositePoint{ wall.p1.x + 5.0f + 0.35f * (13 - q), wall.p1.y - 5.0f + 0.35f * (13 - q), _height + 9.5 };


			newPolygon.points.push_back({ wall.p1.x, wall.p1.y, _height });
			newPolygon.points.push_back({ wall.p2.x, wall.p2.y, _height });
			newPolygon.points.push_back({ secondOppositePoint.x, secondOppositePoint.y, secondOppositePoint.z });
			newPolygon.points.push_back({ firstOppositePoint.x, firstOppositePoint.y, firstOppositePoint.z });

			newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
			newPolygon.texturePoints.push_back({ q == 0 ? 2.0f : 1.0f, 0.0 });
			newPolygon.texturePoints.push_back({ q == 0 ? 2.0f : 1.0f, 3.0f });
			newPolygon.texturePoints.push_back({ 0.0f, 3.0f });

			newPolygon.noOfPoints = newPolygon.texturePoints.size();
			newPolygon.idTexture = textureManager->textures[static_cast<int>(Textures::glass_elevation)].idTexture;
			newPolygon.color = Color::white();

			auto newColor = wall.color.mixColor(selectedColor);
			newPolygon.additionalColor = wall.color.mixColor(newColor);

			polygons.push_back(newPolygon);
		}
	}
	else if (getId() == 101215218)
	{
		Wall additionalWall
		{
			{401.83145141601563, -117.92674255371094 },
			{393.61627197265625, -138.16880798339844 },
			Color::white(),
			0,
			Textures::grunwaldzki,
			0,
			0,
			0
		};

		additionalWall.wallLenght = additionalWall.p1.distance2D(additionalWall.p2);

		Polygon newPolygon;

		newPolygon.points.push_back({ additionalWall.p1.x, additionalWall.p1.y, _min_height });
		newPolygon.points.push_back({ additionalWall.p2.x, additionalWall.p2.y, _min_height });
		newPolygon.points.push_back({ additionalWall.p2.x, additionalWall.p2.y, _height });
		newPolygon.points.push_back({ additionalWall.p1.x, additionalWall.p1.y, _height });

		additionalWall.xRatio = static_cast<int>(additionalWall.wallLenght / textureManager->textures[static_cast<long>(additionalWall.textureName)].realWidth);
		additionalWall.yRatio = static_cast<int>(_height - _min_height) / textureManager->textures[static_cast<long>(additionalWall.textureName)].realHeight;


		newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
		newPolygon.texturePoints.push_back({ additionalWall.xRatio, 0.0 });
		newPolygon.texturePoints.push_back({ additionalWall.xRatio, additionalWall.yRatio });
		newPolygon.texturePoints.push_back({ 0.0f, additionalWall.yRatio });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = textureManager->textures[static_cast<int>(additionalWall.textureName)].idTexture;
		newPolygon.color = Color::white();

		auto newColor = additionalWall.color.mixColor(selectedColor);
		newPolygon.additionalColor = additionalWall.color.mixColor(newColor);

		polygons.push_back(newPolygon);

	}

}