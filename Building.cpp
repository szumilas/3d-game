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

		polygons.push_back( newPolygon );
		conditionalPolygons.push_back({ polygons.size() - 1, wall.angle });
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
		newWall.angle = pow(2, static_cast<int>(shadeTheWall(newWall.color, wallLine, shadePower) / (2 * PI) * 12) % 12);

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
			if (!_custom_texture.empty())
			{
				genericWallTexture = true;

				auto textureName = TextureManager::Instance()->getTextureFromFilePath(_custom_texture);

				switch (textureName)
				{
				case Textures::no_texture:
					emptyWallTextureName = Textures::no_texture;
					smallWallTextureName = Textures::no_texture;
					bigWallTextureName = Textures::no_texture;
					break;

				case Textures::building_00_big:
					emptyWallTextureName = Textures::building_00_empty;
					smallWallTextureName = Textures::building_00_small;
					bigWallTextureName = Textures::building_00_big;
					break;

				case Textures::building_01_big:
					emptyWallTextureName = Textures::building_01_empty;
					smallWallTextureName = Textures::building_01_small;
					bigWallTextureName = Textures::building_01_big;
					break;

				case Textures::building_02_big:
					emptyWallTextureName = Textures::building_02_empty;
					smallWallTextureName = Textures::building_02_small;
					bigWallTextureName = Textures::building_02_big;
					break;

				case Textures::building_03_big:
					emptyWallTextureName = Textures::building_03_empty;
					smallWallTextureName = Textures::building_03_small;
					bigWallTextureName = Textures::building_03_big;
					break;

				case Textures::building_04_big:
					emptyWallTextureName = Textures::building_04_empty;
					smallWallTextureName = Textures::building_04_small;
					bigWallTextureName = Textures::building_04_big;
					break;

				case Textures::building_05_big:
					emptyWallTextureName = Textures::building_05_empty;
					smallWallTextureName = Textures::building_05_small;
					bigWallTextureName = Textures::building_05_big;
					break;

				case Textures::building_06_big:
					emptyWallTextureName = Textures::building_06_empty;
					smallWallTextureName = Textures::building_06_small;
					bigWallTextureName = Textures::building_06_big;
					break;

				case Textures::building_07_big:
					emptyWallTextureName = Textures::building_07_empty;
					smallWallTextureName = Textures::building_07_small;
					bigWallTextureName = Textures::building_07_big;
					break;

				case Textures::building_08_big:
					emptyWallTextureName = Textures::building_08_empty;
					smallWallTextureName = Textures::building_08_small;
					bigWallTextureName = Textures::building_08_big;
					break;

				default:
					emptyWallTextureName = textureName;
					smallWallTextureName = textureName;
					bigWallTextureName = textureName;
					break;
				}
			}
			else
			{
				if (abs(getId()) % 10 == 0)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_00_empty;
					smallWallTextureName = Textures::building_00_small;
					bigWallTextureName = Textures::building_00_big;
				}
				else if (abs(getId()) % 10 == 1)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_01_empty;
					smallWallTextureName = Textures::building_01_small;
					bigWallTextureName = Textures::building_01_big;
				}
				else if (abs(getId()) % 10 == 2)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_02_empty;
					smallWallTextureName = Textures::building_02_small;
					bigWallTextureName = Textures::building_02_big;
				}
				else if (abs(getId()) % 10 == 3)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_03_empty;
					smallWallTextureName = Textures::building_03_small;
					bigWallTextureName = Textures::building_03_big;
				}
				else if (abs(getId()) % 10 == 4)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_04_empty;
					smallWallTextureName = Textures::building_04_small;
					bigWallTextureName = Textures::building_04_big;
				}
				else if (abs(getId()) % 10 == 5)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_05_empty;
					smallWallTextureName = Textures::building_05_small;
					bigWallTextureName = Textures::building_05_big;
				}
				else if (abs(getId()) % 10 == 6)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_06_empty;
					smallWallTextureName = Textures::building_06_small;
					bigWallTextureName = Textures::building_06_big;
				}
				else if (abs(getId()) % 10 == 7)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_07_empty;
					smallWallTextureName = Textures::building_07_small;
					bigWallTextureName = Textures::building_07_big;
				}
				else if (abs(getId()) % 10 == 8)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_05_empty;
					smallWallTextureName = Textures::building_05_small;
					bigWallTextureName = Textures::building_05_big;
				}
				else if (abs(getId()) % 10 == 9)
				{
					genericWallTexture = true;
					emptyWallTextureName = Textures::building_04_empty;
					smallWallTextureName = Textures::building_04_small;
					bigWallTextureName = Textures::building_04_big;
				}
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

std::vector<std::pair<char*, char*>> Building::getObjectXMLTags()
{
	static std::vector<std::pair<char*, char*>>XMLTags = {
		{ "k", "building" },
		{ "v", "yes" }
	};

	return XMLTags;
}
std::vector<std::vector<std::pair<char*, std::string>>> Building::getObjectXMLExtraTags()
{
	std::vector<std::vector<std::pair<char*, std::string>>>XMLExtraTags = {
		{ { "k", "building:colour" },{ "v", _color.getColorHex() } },
		{ { "k", "height" },{ "v", std::to_string(_height) } },
		{ { "k", "min_height" },{ "v", std::to_string(_min_height) } },
	};

	return XMLExtraTags;
}

void Building::recalculateFinalGeometry()
{
	walls.clear();
	generateWalls();

	polygons.clear();
	conditionalPolygons.clear();
	calculateFinalGeometry();
}

void Building::display()
{
	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		for (auto& conditionalPolygon : conditionalPolygons)
		{
			if (!conditionalPolygon.angle & cameraAngleFlag)
				continue;

			auto* polygon = &polygons[conditionalPolygon.idPolygon];
			

			glBindTexture(GL_TEXTURE_2D, polygon->idTexture);

			glBegin(GL_POLYGON);
			glColor3f(polygon->color.red, polygon->color.green, polygon->color.blue);

			for (unsigned int i = 0; i < polygon->noOfPoints; i++)
			{
				glTexCoord2f(polygon->texturePoints[i].x, polygon->texturePoints[i].y);
				glVertex3f(polygon->points[i].x, polygon->points[i].y, polygon->points[i].z);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

long Building::cameraAngleFlag;

void Building::setCameraAngleFlag(std::pair<Point, Point>& camera)
{
	Point& center = camera.first;
	Point& lookAt = camera.second;

	vector2D line(center, lookAt);

	auto directedAngle = vector2D::directedAngle(line, vector2D({ 0, 0 }, { 1, 0 }));
	
	auto origin12Angle = static_cast<int>(directedAngle / (2 * PI) * 12);
	origin12Angle %= 12;

	cameraAngleFlag = 0;

	for (int q = (origin12Angle - 3 + 12) % 12; q != (origin12Angle + 4) % 12; q = (q + 1) % 12)
	{
		cameraAngleFlag += pow(2, q);
	}
}