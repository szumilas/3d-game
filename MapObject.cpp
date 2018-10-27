#include "MapObject.h"

void MapObject::display()
{

}

void MapObject::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	for (auto& ref : refs)
	{
		Point newPoint;
		newPoint.x = nodes.at(ref).posX;
		newPoint.y = nodes.at(ref).posY;
		points.push_back(newPoint);
	}
}

void MapObject::applyKnownValues()
{
	if (!height.empty()) { _height = std::stof(height); };
	if (!min_height.empty()) { _min_height = std::stof(min_height); };
	if (!colour.empty())
	{
		if (colour[0] == '#')
		{
			_red = static_cast<float>(std::stoul(colour.substr(1, 2), nullptr, 16)) / 256;
			_green = static_cast<float>(std::stoul(colour.substr(3, 2), nullptr, 16)) / 256;
			_blue = static_cast<float>(std::stoul(colour.substr(5, 2), nullptr, 16)) / 256;
		}
		else
		{
			auto mapElement = colorMap.find(colour);
			if (mapElement != colorMap.end())
			{
				_red = std::get<0>(mapElement->second);
				_green = std::get<1>(mapElement->second);
				_blue = std::get<2>(mapElement->second);
			}
			else
			{
				std::string error = "unknown color!";
			}
		}
	};
}