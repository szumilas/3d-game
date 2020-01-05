#include "Skybox.h"

Skybox::Skybox(float minX, float maxX, float minY, float maxY)
{
	MapObject::minX = minX;
	MapObject::maxX = maxX;
	MapObject::minY = minY;
	MapObject::maxY = maxY;

	calculateXYfromRef();
	calculateFinalGeometry();

	selectable = false;
}

void Skybox::calculateXYfromRef()
{
	points.push_back(Point{ minX - 0, minY - 500, 0 });
	points.push_back(Point{ minX - 0, maxY + 500, 0 });
	points.push_back(Point{ maxX + 0, maxY + 500, 0 });
	points.push_back(Point{ maxX + 0, minY - 500, 0 });


	/*points.push_back(Point{ minX, minY, 0 });
	points.push_back(Point{ minX, maxY, 0 });
	points.push_back(Point{ maxX, maxY, 0 });
	points.push_back(Point{ maxX, minY, 0 });

	for (double q = minX; q < maxX; q += (maxX - minX) / 64)
	{
		points.push_back(Point{ q, minY, 0 });
		points.push_back(Point{ q, maxY, 0 });
	}

	for (double q = minY; q < maxY; q += (maxY - minY) / 64)
	{
		points.push_back(Point{ minX, q, 0 });
		points.push_back(Point{ maxX, q, 0 });
	}*/
}

void Skybox::calculateFinalGeometry()
{
	for (size_t q = 0; q < 4; q++)
	{
		auto& firstPoint = points[q];
		auto& secondPoint = points[(q + 1) % 4];

		Polygon newPolygon;

		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, -skyboxHeight });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, -skyboxHeight });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, skyboxHeight });
		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, skyboxHeight });

		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.3333f });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.3333f });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.6666f });
		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.6666f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::skybox)].idTexture;
		newPolygon.color = Color{1,1,1};
		
		polygons.push_back(newPolygon);
	}

	/*for (size_t q = 0; q < 4; q++)
	{
		auto& firstPoint = points[4 + q];
		auto& secondPoint = points[4 + (q + 1) % 4];

		Polygon newPolygon;

		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, 0 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, 0 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, 20 });
		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, 20 });

		newPolygon.texturePoints.push_back({ 0.00f, 0.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 0.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 1.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 0.00f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();

		newPolygon.idTexture = textureManager->textures[static_cast<long>(Textures::no_texture)].idTexture;
		newPolygon.color = Color{ 1,1,0 };
		polygons.push_back(newPolygon);
	}


	for (int q = 8; q < points.size(); q += 2)
	{
		auto& firstPoint = points[q];
		auto& secondPoint = points[q + 1];

		Polygon newPolygon;

		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, 0 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, 0 });
		newPolygon.points.push_back({ secondPoint.x, secondPoint.y, 20 });
		newPolygon.points.push_back({ firstPoint.x, firstPoint.y, 20 });

		newPolygon.texturePoints.push_back({ 0.00f, 0.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 0.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 1.00f });
		newPolygon.texturePoints.push_back({ 1.00f, 0.00f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();

		newPolygon.idTexture = textureManager->textures[static_cast<long>(Textures::no_texture)].idTexture;

		if((q + 8) % 16 == 0)
			newPolygon.color = Color{ 0,1,1 };
		else
			newPolygon.color = Color{ 1,0,1 };

		polygons.push_back(newPolygon);

	}*/


	{
		//bottom
		Polygon newPolygon;
		newPolygon.points.push_back({ points[0].x, points[0].y, -skyboxHeight });
		newPolygon.points.push_back({ points[1].x, points[1].y, -skyboxHeight });
		newPolygon.points.push_back({ points[2].x, points[2].y, -skyboxHeight });
		newPolygon.points.push_back({ points[3].x, points[3].y, -skyboxHeight });

		newPolygon.texturePoints.push_back({ 0.26f, 0.00f });
		newPolygon.texturePoints.push_back({ 0.49f, 0.00f });
		newPolygon.texturePoints.push_back({ 0.49f, 0.32f });
		newPolygon.texturePoints.push_back({ 0.26f, 0.32f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::skybox)].idTexture;
		newPolygon.color = Color{ 1,1,1 };

		polygons.push_back(newPolygon);
	}
	{
		//top
		Polygon newPolygon;
		newPolygon.points.push_back({ points[0].x, points[0].y, skyboxHeight });
		newPolygon.points.push_back({ points[1].x, points[1].y, skyboxHeight });
		newPolygon.points.push_back({ points[2].x, points[2].y, skyboxHeight });
		newPolygon.points.push_back({ points[3].x, points[3].y, skyboxHeight });

		newPolygon.texturePoints.push_back({ 0.26f, 1.00f });
		newPolygon.texturePoints.push_back({ 0.26f, 0.6666f });
		newPolygon.texturePoints.push_back({ 0.49f, 0.6666f });
		newPolygon.texturePoints.push_back({ 0.49f, 1.00f });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::skybox)].idTexture;
		newPolygon.color = Color{ 1,1,1 };

		polygons.push_back(newPolygon);
	}
}