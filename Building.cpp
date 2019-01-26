#include "Building.h"

Building::Building(MapObject& mapObject) : MapObject(mapObject)
{

	if (building == "roof" || building_part == "roof")
	{
		if (!_height)
		{
			_height = 6.5f;
		}
		_min_height = std::min(6.0f, _height - 0.5f);
	}
	if (!_height)
	{
		_height = 15.0f;
	}
	if (colour.empty())
	{
		_color.red = 0.5f;
		_color.green = 0.5f;
		_color.blue = 0.5f;
	}
};

void Building::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	MapObject::optimizePoints();

	generateWalls();
}

void Building::calculateFinalGeometry(TextureManager* textureManager)
{
	for (auto& wall : walls)
	{
		wall.idTexture = textureManager->textures[static_cast<long>(wall.textureName)].idTexture;
		wall.xRatio = static_cast<int>(wall.wallLenght / textureManager->textures[static_cast<long>(wall.textureName)].realWidth);
		wall.yRatio = static_cast<int>(_height - _min_height) / textureManager->textures[static_cast<long>(wall.textureName)].realHeight;

		if (!wall.xRatio)
			wall.xRatio = 1.0f;

		if (!wall.yRatio)
			wall.yRatio = 1.0f;

	}

	for (auto& wall : walls)
	{
		Polygon newPolygon;

		newPolygon.points.push_back({ wall.p1.x, wall.p1.y, _min_height });
		newPolygon.points.push_back({ wall.p2.x, wall.p2.y, _min_height });
		newPolygon.points.push_back({ wall.p2.x, wall.p2.y, _height });
		newPolygon.points.push_back({ wall.p1.x, wall.p1.y, _height });

		newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
		newPolygon.texturePoints.push_back({ wall.xRatio, 0.0 });
		newPolygon.texturePoints.push_back({ wall.xRatio, wall.yRatio });
		newPolygon.texturePoints.push_back({ 0.0f, wall.yRatio });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = wall.idTexture;
		newPolygon.color = wall.color;

		auto newColor = wall.color.mixColor(selectedColor);
		newPolygon.additionalColor = wall.color.mixColor(newColor);

		polygons.push_back(newPolygon);
	}
}

void Building::generateWalls()
{
	vector2D equator(Point(0.0, 0.0), Point(1.0, 0.0));

	for (int q = 0; q < static_cast<int>(points.size()) - 1; q++)
	{
		Wall newWall;
		newWall.p1 = points[q];
		newWall.p2 = points[q + 1];

		newWall.color.red = _color.red;
		newWall.color.green = _color.green;
		newWall.color.blue = _color.blue;

		vector2D wallLine(newWall.p1, newWall.p2);
		shadeTheWall(newWall.color, wallLine, 0.5f);

		newWall.wallLenght = wallLine.length();

		walls.push_back(newWall);
	}

	for (auto& wall : walls)
	{
		Textures wallTexture = Textures::apartment_windows;

		if (building == "yes" && _height == 15)
		{
			if (wall.wallLenght < 4)
			{
				wallTexture = Textures::tenement_house_no_windows;
			}
			else
			{
				wallTexture = Textures::tenement_house_windows;
			}
		}
		else if (building == "office")
		{
			wallTexture = Textures::office_windows;
		}
		else
		{
			if (wall.wallLenght < 4)
			{
				wallTexture = Textures::apartment_no_windows;
			}
			else if (wall.wallLenght >= 4 && wall.wallLenght < 200)
			{
				wallTexture = Textures::apartment_one_window;
			}
			else
			{
				wallTexture = Textures::apartment_windows;
			}
		}

		wall.textureName = wallTexture;
	}
}