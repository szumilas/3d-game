#include "BusShelter.h"

BusShelter::BusShelter(MapObject& mapObject) : Building(mapObject)
{
	_height = 3.9f;
	_min_height = 0.0f;

	roof_shape = "flat";

	roofNeeded = false;
};

void BusShelter::calculateFinalGeometry(TextureManager* textureManager)
{
	Building::calculateFinalGeometry(textureManager);

	//TODO: add better roof
	Polygon newPolygon;

	newPolygon.points.push_back({ walls[0].p1.x, walls[0].p1.y, 2.4f });
	newPolygon.points.push_back({ walls[0].p2.x, walls[0].p2.y, 2.4f });
	newPolygon.points.push_back({ walls[1].p1.x, walls[1].p1.y, 2.4f });
	newPolygon.points.push_back({ walls[1].p2.x, walls[1].p2.y, 2.4f });

	newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
	newPolygon.texturePoints.push_back({ 1.0f, 0.0 });
	newPolygon.texturePoints.push_back({ 1.0f, 1.0f });
	newPolygon.texturePoints.push_back({ 0.0f, 1.0f });

	newPolygon.noOfPoints = newPolygon.texturePoints.size();
	newPolygon.color = Color{};

	polygons.push_back(newPolygon);
}

void BusShelter::generateWalls()
{
	Building::generateWalls();

	std::sort(walls.begin(), walls.end(), [](Wall& a, Wall& b) {return a.wallLenght < b.wallLenght; });

	walls.resize(2);

	walls[0].textureName = Textures::bus_shelter_with_sign;
	walls[1].textureName = Textures::bus_shelter;

	walls[0].color = Color{1.0f, 1.0f, 1.0f};
	walls[1].color = Color{1.0f, 1.0f, 1.0f};

}