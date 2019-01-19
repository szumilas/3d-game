#include "Skybox.h"

Skybox::Skybox(TextureManager* textureManager, float minX, float maxX, float minY, float maxY)
{
	MapObject::minX = minX;
	MapObject::maxX = maxX;
	MapObject::minY = minY;
	MapObject::maxY = maxY;

	calculateXYfromRef();
	calculateFinalGeometry(textureManager);
}

void Skybox::calculateXYfromRef()
{
	points.push_back(Point{ minX, minY, 0 });
	points.push_back(Point{ minX, maxY, 0 });
	points.push_back(Point{ maxX, maxY, 0 });
	points.push_back(Point{ maxX, minY, 0 });
}

void Skybox::calculateFinalGeometry(TextureManager* textureManager)
{
	for (size_t q = 0; q < 4; q++)
	{
		auto& firstPoint = points[q];
		auto& secondPoint = points[(q + 1) % 4];

		Polygon newPolygon;

		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, -500 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, -500 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, 500 });
		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, 500 });

		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.34f });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.34f });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.66f });
		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.66f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = textureManager->textures[static_cast<long>(Textures::skybox)].idTexture;
		newPolygon.color = Color{1,1,1};
		
		polygons.push_back(newPolygon);
	}

	Polygon newPolygon;

	newPolygon.points.push_back({ points[0].x, points[0].y, -500 });
	newPolygon.points.push_back({ points[1].x, points[1].y, -500 });
	newPolygon.points.push_back({ points[2].x, points[2].y, -500 });
	newPolygon.points.push_back({ points[3].x, points[3].y, -500 });

	newPolygon.texturePoints.push_back({ 0.26f, 0.00f });
	newPolygon.texturePoints.push_back({ 0.49f, 0.00f });
	newPolygon.texturePoints.push_back({ 0.49f, 0.32f });
	newPolygon.texturePoints.push_back({ 0.26f, 0.32f });

	newPolygon.noOfPoints = newPolygon.texturePoints.size();
	newPolygon.idTexture = textureManager->textures[static_cast<long>(Textures::skybox)].idTexture;
	newPolygon.color = Color{ 1,1,1 };

	polygons.push_back(newPolygon);

}