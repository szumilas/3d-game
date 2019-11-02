#include "Tree.h"

Tree::Tree(long long ref) : MapObject(ref)
{
	refs.push_back(ref);
	collidable = Collidable::point;
}

void Tree::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	//importFromObjFile("Data/Trees/elm_tree.obj", Textures::elm_tree, 0.5);

	Point newPoint;
	
	newPoint.x = nodes.at(refs[0]).posX;
	newPoint.y = nodes.at(refs[0]).posY;

	auto random1 = static_cast<float>(static_cast<int>((abs(newPoint.x) * 100) + static_cast<int>(abs(newPoint.y) * 100)) % 100) / 100;
	auto random2 = static_cast<float>(static_cast<int>((abs(newPoint.x) * 1000) + static_cast<int>(abs(newPoint.y) * 1000)) % 100) / 100;
	auto random3 = static_cast<float>(static_cast<int>((abs(newPoint.x) * 10) + static_cast<int>(abs(newPoint.y) * 10)) % 100) / 100;
	auto random4 = static_cast<int>(static_cast<int>((abs(newPoint.x) * 10) + static_cast<int>(abs(newPoint.y) * 100)) % 100);

	if (_height == 0)
	{
		_height = 4.0 + random1 * 2.0;
	}

	_color = Color(0.7 + random1 * 0.3, 0.7 + random2 * 0.3, 0.7 + random3 * 0.3);

	position = newPoint;

	std::vector<std::pair<Point, Point>> treePoints{ {{ newPoint.x - _height / 2, newPoint.y }, { newPoint.x + _height / 2, newPoint.y, 0 }},
	{ { newPoint.x, newPoint.y - _height / 2},{ newPoint.x, newPoint.y + _height / 2, 0 } } };

	for (auto& line : treePoints)
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

		if(random4 % 2 == 0)
			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::elm_tree)].idTexture;
		else
			polygon.idTexture = TextureManager::Instance()->textures[static_cast<unsigned int>(Textures::tree)].idTexture;

		polygons.push_back(polygon);
	}
}