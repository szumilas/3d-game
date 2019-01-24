#include "Riverbank.h"

Riverbank::Riverbank(MapObject& mapObject) : MapObject(mapObject)
{
	_height = 0.0f;
	_min_height = -3.0f;

	_color.red = 0.8f;
	_color.green = 0.8f;
	_color.blue = 0.8f;
};

void Riverbank::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	MapObject::epsilon = 5.0;
	MapObject::optimizePoints();

	generateWalls();
}

void Riverbank::calculateFinalGeometry(TextureManager* textureManager)
{
	for (auto& wall : walls)
	{
		
		auto wallTexture = Textures::retaining_wall;

		wall.idTexture = textureManager->textures[static_cast<long>(wallTexture)].idTexture;
		wall.xRatio = static_cast<int>(wall.wallLenght / textureManager->textures[static_cast<long>(wallTexture)].realWidth);
		wall.yRatio = static_cast<int>(_height - _min_height) / textureManager->textures[static_cast<long>(wallTexture)].realHeight;

		if (!wall.xRatio)
			wall.xRatio = 1.0f;

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

void Riverbank::generateWalls()
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
}