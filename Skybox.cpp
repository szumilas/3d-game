#include "Skybox.h"

Skybox::Skybox(TextureManager* textureManager)
{

}

void Skybox::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	points.push_back(Point{ -500.0f, -500.0f, 0 });
	points.push_back(Point{ -500.0f,  500.0f, 0 });
	points.push_back(Point{  500.0f,  500.0f, 0 });
	points.push_back(Point{  500.0f, -500.0f, 0 });
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

		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.33f });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.33 });
		newPolygon.texturePoints.push_back({ 0.25f + static_cast<float>(q) / 4, 0.66 });
		newPolygon.texturePoints.push_back({ 0.00f + static_cast<float>(q) / 4, 0.66 });

		newPolygon.noOfPoints = newPolygon.texturePoints.size();
		newPolygon.idTexture = textureManager->textures[static_cast<long>(Textures::skybox)].idTexture;
		newPolygon.color = Color{1,1,1};
		
		polygons.push_back(newPolygon);
	}
}