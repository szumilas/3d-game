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
			float yRatio = static_cast<int>((_height - _min_height) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realHeight);

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio || barrier == "hedge")
				yRatio = 1.0f;

			if (yRatio == 1.0f)
				yRatio = 0.95f;

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ xRatio, 0 });
			polygon.texturePoints.push_back({ xRatio, yRatio });
			polygon.texturePoints.push_back({ 0, yRatio });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = _color;

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;

			polygons.push_back(polygon);

		}
	}

	auto topLevel = _height;

	if (barrier == "hedge")
		topLevel *= 0.9;

	if (sidesOfBarrier.size() == 2)
	{
		std::vector<Point>& leftSide = sidesOfBarrier[0];
		std::vector<Point>& rightSide = sidesOfBarrier[1];

		//top of barrier

		for (size_t q = 0; q < leftSide.size() - 1; q++)
		{
			Polygon polygon;
			polygon.points.push_back({ leftSide[q].x, leftSide[q].y, topLevel });
			polygon.points.push_back({ leftSide[q + 1].x, leftSide[q + 1].y, topLevel });
			polygon.points.push_back({ rightSide[q + 1].x, rightSide[q + 1].y, topLevel });
			polygon.points.push_back({ rightSide[q].x, rightSide[q].y, topLevel });

			float xRatio = static_cast<int>(leftSide[q].distance2D(leftSide[q + 1]) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realWidth);
			float yRatio = _width / TextureManager::Instance()->textures[static_cast<long>(textureName)].realHeight;

			if (!xRatio)
				xRatio = 1.0f;
			if (!yRatio)
				yRatio = 1.0f;

			if (barrier == "hedge")
			{
				polygon.texturePoints.push_back({ 0, 0.2 });
				polygon.texturePoints.push_back({ xRatio, 0.2 });
				polygon.texturePoints.push_back({ xRatio, yRatio });
				polygon.texturePoints.push_back({ 0, yRatio });
			}
			else
			{
				polygon.texturePoints.push_back({ 0, 0 });
				polygon.texturePoints.push_back({ xRatio, 0 });
				polygon.texturePoints.push_back({ xRatio, yRatio });
				polygon.texturePoints.push_back({ 0, yRatio });
			}

			polygon.noOfPoints = polygon.points.size();
			polygon.color = _color;

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;

			polygons.push_back(polygon);
		}

		//beginning and end of the barrier
		{
			for (std::pair<Point, Point>& points : std::vector<std::pair<Point, Point>>{ { leftSide.front(), rightSide.front() }, { leftSide.back(), rightSide.back() } })
			{
				Polygon polygon;
				polygon.points.push_back({ points.first.x, points.first.y, _min_height });
				polygon.points.push_back({ points.second.x, points.second.y, _min_height });
				polygon.points.push_back({ points.second.x, points.second.y, _height });
				polygon.points.push_back({ points.first.x, points.first.y, _height });

				float xRatio = static_cast<int>(_width / TextureManager::Instance()->textures[static_cast<long>(textureName)].realWidth);
				float yRatio = static_cast<int>((_height - _min_height) / TextureManager::Instance()->textures[static_cast<long>(textureName)].realHeight);

				if (!xRatio)
					xRatio = 1.0f;
				if (!yRatio || barrier == "hedge")
					yRatio = 1.0f;

				if (yRatio == 1.0f)
					yRatio = 0.95f;

				polygon.texturePoints.push_back({ 0, 0 });
				polygon.texturePoints.push_back({ xRatio, 0 });
				polygon.texturePoints.push_back({ xRatio, yRatio });
				polygon.texturePoints.push_back({ 0, yRatio });

				polygon.noOfPoints = polygon.points.size();
				polygon.color = _color;

				polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(textureName)].idTexture;

				polygons.push_back(polygon);
			}
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
		if (barrier == "wall") { _color.red = 170.0f / 256; _color.green = 113.0f / 256; _color.blue = 82.0f / 256; }
		else if (barrier == "kerb") { _color.red = 241.0f / 256; _color.green = 235.0f / 256; _color.blue = 240.0f / 256; }
		else if (barrier == "hedge") { _color.red = 140.0f / 256; _color.green = 195.0f / 256; _color.blue = 2.0f / 256; }
		else if (barrier == "bollard") { _color.red = 234.0f / 256; _color.green = 207.0f / 256; _color.blue = 32.0f / 256; }
		else if (barrier == "retaining_wall") { _color.red = 213.0f / 256; _color.green = 205.0f / 256; _color.blue = 195.0f / 256; }
		else if (barrier == "guard_rail") { _color.red = 179.0f / 256; _color.green = 225.0f / 256; _color.blue = 251.0f / 256; }
		else if (barrier == "fence") { _color.red = 171.0f / 256; _color.green = 187.0f / 256; _color.blue = 197.0f / 256; }
		else if (barrier == "race_barrier") { _color.red = 171.0f / 256; _color.green = 187.0f / 256; _color.blue = 197.0f / 256; }
		else { _color.red = 128.0f / 256; _color.green = 128.0f / 256; _color.blue = 128.0f / 256; }
	}

	if (barrier == "wall") { textureName = Textures::concrete_wall; if(!_width) _width = 0.25f; }
	else if (barrier == "kerb") { textureName = Textures::concrete_wall; if (!_width) _width = 0.1f; }
	else if (barrier == "hedge") { textureName = Textures::hedge; if (!_width) _width = 0.5f; }
	else if (barrier == "bollard") { textureName = Textures::bollard; }
	else if (barrier == "retaining_wall") { textureName = Textures::retaining_wall; if (!_width) _width = 0.2f; }
	else if (barrier == "guard_rail") { textureName = Textures::guard_rail; }
	else if (barrier == "fence") { textureName = Textures::fence; }
	else if (barrier == "race_barrier") { textureName = Textures::race_barrier; }
	else { textureName = Textures::concrete_wall; if (!_width) _width = 0.2f; }

	if (!_custom_texture.empty())
	{
		textureName = TextureManager::Instance()->getTextureFromFilePath(_custom_texture);
	}
}