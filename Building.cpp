#include "Building.h"
#include <typeinfo>

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
		if (building == "roof")
		{
			_color = Color(ColorName::BLACK);
		}
		if (typeid(*this) == typeid(Building) && building != "office")
		{
			if ((getId() / 10) % 10 == 0)
			{
				_color = Color{ 1.0f, 1.0f, 1.0f };
			}
			else if ((getId() / 10) % 10 == 1)
			{
				_color = Color{ 1.0f, 1.0f, 0.9f };

			}
			else if ((getId() / 10) % 10 == 2)
			{
				_color = Color{ 1.0f, 0.9f, 1.0f };
			}
			else if ((getId() / 10) % 10 == 3)
			{
				_color = Color{ 1.0f, 0.9f, 0.9f };
			}
			else if ((getId() / 10) % 10 == 4)
			{
				_color = Color{ 0.9f, 1.0f, 1.0f };
			}
			else if ((getId() / 10) % 10 == 5)
			{
				_color = Color{ 0.9f, 1.0f, 0.9f };
			}
			else if ((getId() / 10) % 10 == 6)
			{
				_color = Color{ 0.9f, 0.9f, 1.0f };
			}
			else if ((getId() / 10) % 10 == 7)
			{
				_color = Color{ 0.9f, 0.9f, 0.9f };
			}
			else if ((getId() / 10) % 10 == 8)
			{
				_color = Color{ 0.9f, 1.0f, 0.96f };
			}
			else if ((getId() / 10) % 10 == 9)
			{
				_color = Color{ 0.9f, 0.95f, 1.0f };
			}
		}
		else
		{
			_color = Color(ColorName::GRAY);
		}
	}

	if(_min_height < 0.5)
		collidable = Collidable::polygon;
};

void Building::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	MapObject::optimizePoints();

	generateWalls();

	if (points.size() <= 3)
	{
		_skip = "yes";
	}
}

void Building::calculateFinalGeometry()
{
	if (genericWallTexture)
	{
		applyGenericTextures();
	}

	for (auto& wall : walls)
	{
		wall.idTexture = TextureManager::Instance()->textures[static_cast<long>(wall.textureName)].idTexture;
		if(!wall.xRatio)
			wall.xRatio = round(wall.wallLenght / TextureManager::Instance()->textures[static_cast<long>(wall.textureName)].realWidth);
		if (!wall.yRatio)
			wall.yRatio = round(_height - _min_height) / TextureManager::Instance()->textures[static_cast<long>(wall.textureName)].realHeight;

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
		shadeTheWall(newWall.color, wallLine, shadePower);

		newWall.wallLenght = wallLine.length();

		walls.push_back(newWall);
	}

	for (auto& wall : walls)
	{
		Textures wallTexture = Textures::no_texture;

		if (building == "office")
		{
			wallTexture = Textures::office_windows;
		}
		else if (building == "roof")
		{
			wallTexture = Textures::no_texture;
		}
		else if (typeid(*this) == typeid(Building))
		{
			if (getId() % 10 == 0)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_00_empty;
				smallWallTextureName = Textures::building_00_small;
				bigWallTextureName = Textures::building_00_big;
			}
			else if (getId() % 10 == 1)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_01_empty;
				smallWallTextureName = Textures::building_01_small;
				bigWallTextureName = Textures::building_01_big;
			}
			else if (getId() % 10 == 2)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_02_empty;
				smallWallTextureName = Textures::building_02_small;
				bigWallTextureName = Textures::building_02_big;
			}
			else if (getId() % 10 == 3)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_03_empty;
				smallWallTextureName = Textures::building_03_small;
				bigWallTextureName = Textures::building_03_big;
			}
			else if (getId() % 10 == 4)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_04_empty;
				smallWallTextureName = Textures::building_04_small;
				bigWallTextureName = Textures::building_04_big;
			}
			else if (getId() % 10 == 5)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_05_empty;
				smallWallTextureName = Textures::building_05_small;
				bigWallTextureName = Textures::building_05_big;
			}
			else if (getId() % 10 == 6)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_06_empty;
				smallWallTextureName = Textures::building_06_small;
				bigWallTextureName = Textures::building_06_big;
			}
			else if (getId() % 10 == 7)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_07_empty;
				smallWallTextureName = Textures::building_07_small;
				bigWallTextureName = Textures::building_07_big;
			}
			else if (getId() % 10 == 8)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_05_empty;
				smallWallTextureName = Textures::building_05_small;
				bigWallTextureName = Textures::building_05_big;
			}
			else if (getId() % 10 == 9)
			{
				genericWallTexture = true;
				emptyWallTextureName = Textures::building_04_empty;
				smallWallTextureName = Textures::building_04_small;
				bigWallTextureName = Textures::building_04_big;
			}
		}

		wall.textureName = wallTexture;
	}
}

void Building::applyGenericTextures()
{
	for (auto& wall : walls)
	{
		auto exactBigTextureRatio = wall.wallLenght / TextureManager::Instance()->textures[static_cast<long>(bigWallTextureName)].realWidth;
		auto roundedBigTextureRation = round(exactBigTextureRatio);

		if (roundedBigTextureRation)
		{
			if (exactBigTextureRatio < roundedBigTextureRation)
			{
				std::swap(exactBigTextureRatio, roundedBigTextureRation);
			}

			if (exactBigTextureRatio / roundedBigTextureRation < 1.10)
			{
				wall.textureName = bigWallTextureName;
				continue;
			}
		}

		auto exactSmallTextureRatio = wall.wallLenght / TextureManager::Instance()->textures[static_cast<long>(smallWallTextureName)].realWidth;
		auto roundedSmallTextureRation = round(exactSmallTextureRatio);

		if (roundedSmallTextureRation)
		{
			if (exactSmallTextureRatio < roundedSmallTextureRation)
			{
				std::swap(exactSmallTextureRatio, roundedSmallTextureRation);
			}

			if (exactSmallTextureRatio / roundedSmallTextureRation < 1.15)
			{
				wall.textureName = smallWallTextureName;
				continue;
			}
		}
		

		wall.textureName = emptyWallTextureName;
	}

}