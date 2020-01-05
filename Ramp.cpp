#include "Ramp.h"

Ramp::Ramp(MapObject& mapObject) : MapObject(mapObject)
{
	if (height.empty())
	{
		_height = 5.0f;
	}
	if (!_min_height)
	{
		_min_height = 0.0f;
	}

	if(colour.empty())
		_color = Color(ColorName::WHITE);

	if (_min_height < 0.5 && _min_height > -0.5f)
		collidable = Collidable::polygon;

	drawingPriority = DrawingPriority::whenClose;
}

void Ramp::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	if (points.size() % 2 == 1)
	{
		points.pop_back();
	}

	originalPoints = points;
}

void Ramp::calculateFinalGeometry()
{
	points = originalPoints;

	auto textureName = TextureManager::Instance()->getTextureFromFilePath(_custom_texture);

	for (int q = 0; q < points.size(); q += 2)
	{
		if (q + 3 < points.size())
		{
			Point& b1 = points[q];
			Point& t1 = points[q + 1];
			Point& b2 = points[q + 2];
			Point& t2 = points[q + 3];



			//first polygon
			{
				Polygon newPolygon;
				newPolygon.points.push_back({ b1.x, b1.y, _min_height });
				newPolygon.points.push_back({ t1.x, t1.y, _height });
				newPolygon.points.push_back({ b2.x, b2.y, _min_height });

				auto heightRatio = round(Point(b1.x, b1.y).distance2D(Point(t1.x, t1.y)) / TextureManager::Instance()->textures[static_cast<int>(textureName)].realHeight);
				auto lengthRatio = round(Point(b1.x, b1.y).distance2D(Point(b2.x, b2.y)) / TextureManager::Instance()->textures[static_cast<int>(textureName)].realWidth);
				newPolygon.texturePoints.push_back({ 0.0f, 0.0f });
				newPolygon.texturePoints.push_back({ 0.0f, heightRatio });
				newPolygon.texturePoints.push_back({ lengthRatio, 0.0f });

				newPolygon.noOfPoints = newPolygon.texturePoints.size();
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<int>(textureName)].idTexture;
				newPolygon.color = _color;

				newPolygon.additionalColor = newPolygon.color.mixColor(selectedColor);

				polygons.push_back(newPolygon);
			}

			//second triangle
			{
				Polygon newPolygon;
				newPolygon.points.push_back({ t1.x, t1.y, _height });
				newPolygon.points.push_back({ b2.x, b2.y, _min_height });
				newPolygon.points.push_back({ t2.x, t2.y, _height });

				auto heightRatio = round(Point(t2.x, t2.y).distance2D(Point(b2.x, b2.y)) / TextureManager::Instance()->textures[static_cast<int>(textureName)].realHeight);
				auto lengthRatio = round(Point(t1.x, t1.y).distance2D(Point(t2.x, t2.y)) / TextureManager::Instance()->textures[static_cast<int>(textureName)].realWidth);
				newPolygon.texturePoints.push_back({ 0.0f, heightRatio });
				newPolygon.texturePoints.push_back({ lengthRatio, 0.0 });
				newPolygon.texturePoints.push_back({ lengthRatio, heightRatio });

				newPolygon.noOfPoints = newPolygon.texturePoints.size();
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<int>(textureName)].idTexture;
				newPolygon.color = _color;

				newPolygon.additionalColor = newPolygon.color.mixColor(selectedColor);

				polygons.push_back(newPolygon);
			}
		}
	}

	points.clear();
	for (int q = 0; q < originalPoints.size(); q += 2)
	{
		points.push_back(originalPoints[q]);
	}
	for (int q = originalPoints.size() - 1; q > 0; q -= 2)
	{
		points.push_back(originalPoints[q]);
	}
}

void Ramp::recalculateFinalGeometry()
{
	polygons.clear();
	calculateFinalGeometry();
}

std::vector<std::pair<char*, char*>> Ramp::getObjectXMLTags()
{
	static std::vector<std::pair<char*, char*>>XMLTags = {
		{ "k", "_custom_object" },
		{ "v", "_ramp" }
	};

	return XMLTags;
}

std::vector<std::vector<std::pair<char*, std::string>>> Ramp::getObjectXMLExtraTags()
{
	std::vector<std::vector<std::pair<char*, std::string>>>XMLExtraTags = {
		{ { "k", "colour" },{ "v", _color.getColorHex() } },
		{ { "k", "height" },{ "v", std::to_string(_height) } },
		{ { "k", "min_height" },{ "v", std::to_string(_min_height) } },
		{ { "k", "_custom_texture" },{ "v", _custom_texture } },
	};

	return XMLExtraTags;
}