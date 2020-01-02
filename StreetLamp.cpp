#include "StreetLamp.h"

StreetLamp::StreetLamp(long long ref) : MapObject(ref)
{
	refs.push_back(ref);
	_height = 5.0f;
	size = 0.1;
	_color = Color(ColorName::BLACK);
	collidable = Collidable::point;
}

void StreetLamp::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	Point newPoint;

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	position = newPoint;

	std::vector<std::pair<Point, Point>> lampPoints;
	lampPoints.push_back({ points[0], points[1] });
	lampPoints.push_back({ points[1], points[2] });
	lampPoints.push_back({ points[2], points[3] });
	lampPoints.push_back({ points[3], points[0] });

	for (auto& line : lampPoints)
	{
		{
			Polygon polygon;
			polygon.points.push_back({ line.first.x, line.first.y, 0 });
			polygon.points.push_back({ line.second.x, line.second.y, 0 });
			polygon.points.push_back({ line.second.x, line.second.y, _height });
			polygon.points.push_back({ line.first.x, line.first.y, _height });

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ 1, 0 });
			polygon.texturePoints.push_back({ 1, 1 });
			polygon.texturePoints.push_back({ 0, 1 });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = _color;

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::no_texture)].idTexture;

			polygons.push_back(polygon);
		}

		int lampshadeSize = 5;
		{
			Polygon polygon;

			polygon.points.push_back({ nodes.at(refs[0]).posX, nodes.at(refs[0]).posY, _height - 0.2 });
			polygon.points.push_back({ (line.second.x - nodes.at(refs[0]).posX) * lampshadeSize + nodes.at(refs[0]).posX, (line.second.y - nodes.at(refs[0]).posY) * lampshadeSize + nodes.at(refs[0]).posY, _height + 0.2 });
			polygon.points.push_back({ (line.first.x - nodes.at(refs[0]).posX) * lampshadeSize + nodes.at(refs[0]).posX, (line.first.y - nodes.at(refs[0]).posY) * lampshadeSize + nodes.at(refs[0]).posY, _height + 0.2 });

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ 1, 0 });
			polygon.texturePoints.push_back({ 1, 1 });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = Color(ColorName::WHITE);

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::no_texture)].idTexture;

			polygons.push_back(polygon);
		}

		{
			Polygon polygon;
			polygon.points.push_back({ nodes.at(refs[0]).posX, nodes.at(refs[0]).posY, _height + 0.4 });
			polygon.points.push_back({ (line.second.x - nodes.at(refs[0]).posX) * lampshadeSize + nodes.at(refs[0]).posX, (line.second.y - nodes.at(refs[0]).posY) * lampshadeSize + nodes.at(refs[0]).posY, _height + 0.2 });
			polygon.points.push_back({ (line.first.x - nodes.at(refs[0]).posX) * lampshadeSize + nodes.at(refs[0]).posX, (line.first.y - nodes.at(refs[0]).posY) * lampshadeSize + nodes.at(refs[0]).posY, _height + 0.2 });

			polygon.texturePoints.push_back({ 0, 0 });
			polygon.texturePoints.push_back({ 1, 0 });
			polygon.texturePoints.push_back({ 1, 1 });

			polygon.noOfPoints = polygon.points.size();
			polygon.color = Color(ColorName::BLACK);

			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::no_texture)].idTexture;

			polygons.push_back(polygon);
		}
	}
}

std::vector<std::pair<char*, char*>> StreetLamp::getObjectXMLTags()
{
	static std::vector<std::pair<char*, char*>>XMLTags = {
		{ "k", "highway" },
		{ "v", "street_lamp" }
	};

	return XMLTags;
}