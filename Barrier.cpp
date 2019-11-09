#include "Barrier.h"


Barrier::Barrier(MapObject& mapObject) : MapObject(mapObject)
{
	if (!_min_height) _min_height = 0.0f;
	if (height.empty())
	{
		if (barrier == "wall") { _height = 2.0f; }
		else if (barrier == "kerb") { _height = 0.1f; }
		else if (barrier == "hedge") { _height = 1.0f; }
		else if (barrier == "bollard") { _height = 0.6f; }
		else if (barrier == "retaining_wall") { _height = 1.0f; }
		else if (barrier == "guard_rail") { _height = 0.75f; }
		else if (barrier == "fence") { _height = 1.5f; }
		else { _height = 1.1f; }
	}
	setTexture();

	collidable = Collidable::polygon;
};

void Barrier::calculateFinalGeometry()
{
	std::vector<std::vector<Point>> sidesOfBarrier;

	if (_width)
	{
		std::vector<Point> leftSide;
		std::vector<Point> rightSide;

		createBothRailsForSymmetryAxis(points, leftSide, rightSide, _width);

		sidesOfBarrier.push_back(leftSide);
		sidesOfBarrier.push_back(rightSide);
	}
	else
	{
		sidesOfBarrier.push_back(points);
	}

	for (auto& pointsToDraw : sidesOfBarrier)
	{
		for (size_t q = 0; q < pointsToDraw.size() - 1; q++)
		{
			auto& point = pointsToDraw[q];
			auto& nextPoint = pointsToDraw[q + 1];

			Polygon polygon;
			polygon.points.push_back({ point.x, point.y, _min_height });
			polygon.points.push_back({ nextPoint.x, nextPoint.y, _min_height });
			polygon.points.push_back({ nextPoint.x, nextPoint.y, _height });
			polygon.points.push_back({ point.x, point.y, _height });

			float xRatio = static_cast<int>(point.distance2D(nextPoint) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realWidth);
			float yRatio = static_cast<int>(_height - _min_height) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realHeight;

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio)
				yRatio = 1.0f;

			if (yRatio == 1.0f)
				yRatio = 0.95f;

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ xRatio, 0 });
			polygon.texturePoints.push_back({ xRatio, yRatio });
			polygon.texturePoints.push_back({ 0, yRatio });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = Color{ 1.0f, 1.0f, 1.0f };

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;

			polygons.push_back(polygon);

		}
	}

	if (barrier == "hedge")
		_height *= 0.9;

	if (sidesOfBarrier.size() == 2)
	{
		std::vector<Point>& leftSide = sidesOfBarrier[0];
		std::vector<Point>& rightSide = sidesOfBarrier[1];

		for (size_t q = 0; q < leftSide.size() - 1; q++)
		{
			Polygon polygon;
			polygon.points.push_back({ leftSide[q].x, leftSide[q].y, _height });
			polygon.points.push_back({ leftSide[q + 1].x, leftSide[q + 1].y, _height });
			polygon.points.push_back({ rightSide[q + 1].x, rightSide[q + 1].y, _height });
			polygon.points.push_back({ rightSide[q].x, rightSide[q].y, _height });

			float xRatio = static_cast<int>(leftSide[q].distance2D(leftSide[q + 1]) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realWidth);
			float yRatio = 1;

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio)
				yRatio = 1.0f;

			if (barrier == "hedge")
				yRatio *= 0.8;

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ xRatio, 0 });
			polygon.texturePoints.push_back({ xRatio, yRatio });
			polygon.texturePoints.push_back({ 0, yRatio });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = Color{ 1.0f, 1.0f, 1.0f };

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;

			polygons.push_back(polygon);
		}
	}
}

std::vector<std::pair<char*, char*>> Barrier::getObjectXMLTags()
{
	static std::vector<std::pair<char*, char*>>XMLTags = {
		{ "k", "barrier" },
		{ "v", "_custom" }
	};

	return XMLTags;
}
std::vector<std::vector<std::pair<char*, std::string>>> Barrier::getObjectXMLExtraTags()
{
	std::vector<std::vector<std::pair<char*, std::string>>>XMLExtraTags = {
		{ { "k", "width" },{ "v", std::to_string(_width) } },
		{ { "k", "height" },{ "v", std::to_string(_height) } },
		{ { "k", "min_height" },{ "v", std::to_string(_min_height) } },
		{ { "k", "_custom_texture" },{ "v", _custom_texture } },
	};

	return XMLExtraTags;
}

void Barrier::recalculateFinalGeometry()
{
	setTexture();
	polygons.clear();
	calculateFinalGeometry();
}

void Barrier::setTexture()
{
	if (colour.empty())
	{
		if (barrier == "wall") { _color.red = 170.0f / 256; _color.green = 113.0f / 256; _color.blue = 82.0f / 256; textureName = Textures::concrete_wall; _width = 0.25f; }
		else if (barrier == "kerb") { _color.red = 241.0f / 256; _color.green = 235.0f / 256; _color.blue = 240.0f / 256; textureName = Textures::concrete_wall; _width = 0.1f; }
		else if (barrier == "hedge") { _color.red = 140.0f / 256; _color.green = 195.0f / 256; _color.blue = 2.0f / 256; textureName = Textures::hedge; _width = 0.5f; }
		else if (barrier == "bollard") { _color.red = 234.0f / 256; _color.green = 207.0f / 256; _color.blue = 32.0f / 256; textureName = Textures::bollard; }
		else if (barrier == "retaining_wall") { _color.red = 213.0f / 256; _color.green = 205.0f / 256; _color.blue = 195.0f / 256; textureName = Textures::retaining_wall; _width = 0.2f; }
		else if (barrier == "guard_rail") { _color.red = 179.0f / 256; _color.green = 225.0f / 256; _color.blue = 251.0f / 256; textureName = Textures::guard_rail; }
		else if (barrier == "fence") { _color.red = 171.0f / 256; _color.green = 187.0f / 256; _color.blue = 197.0f / 256; textureName = Textures::fence; }
		else if (barrier == "race_barrier") { _color.red = 171.0f / 256; _color.green = 187.0f / 256; _color.blue = 197.0f / 256; textureName = Textures::race_barrier; }
		else { _color.red = 128.0f / 256; _color.green = 128.0f / 256; _color.blue = 128.0f / 256; textureName = Textures::concrete_wall; }
	}
	if (!_custom_texture.empty())
	{
		textureName = TextureManager::Instance()->getTextureFromFilePath(_custom_texture);
	}
}