#include "CustomArea.h"

CustomArea::CustomArea(MapObject& mapObject) : MapObject(mapObject)
{
	drawingPriority = DrawingPriority::whenClose;
}

void CustomArea::calculateFinalGeometry()
{
	std::for_each(points.begin(), points.end(), [&](auto& p) { p.z = _height; });

	points.push_back(points.front());

	dividePointsPolygonIntoTriangles();

	for (auto& polygon : polygons)
	{
		polygon.color = _color;
		polygon.additionalColor = _color.mixColor(Color(ColorName::VIOLET));
		polygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(TextureManager::Instance()->getTextureFromFilePath(_custom_texture))].idTexture;
	}
}


std::vector<std::pair<char*, char*>> CustomArea::getObjectXMLTags()
{
	static std::vector<std::pair<char*, char*>>XMLTags = {
		{ "k", "_custom_area" },
		{ "v", "yes" }
	};

	return XMLTags;
}
std::vector<std::vector<std::pair<char*, std::string>>> CustomArea::getObjectXMLExtraTags()
{
	std::vector<std::vector<std::pair<char*, std::string>>>XMLExtraTags = {
		{ { "k", "height" },{ "v", std::to_string(_height) } },
		{ { "k", "_custom_texture" },{ "v", _custom_texture } },
		{ { "k", "colour" },{ "v", _color.getColorHex() } },
	};

	return XMLExtraTags;
}