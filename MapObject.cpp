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
	
	optimizePoints();
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

void MapObject::select()
{
	isSelected = true;
	_redCopy = _red;
	_greenCopy = _green;
	_blueCopy = _blue;

	_red = 0.8f;
	_green = 0.2f;
	_blue = 0.5f;
}

void MapObject::deselect()
{
	if (isSelected)
	{
		isSelected = false;
		_red = _redCopy;
		_green = _greenCopy;
		_blue = _blueCopy;
	}

}

void MapObject::optimizePoints()
{
	if (points.size() > 2)
	{
		for (auto it = points.begin() + 1; it + 2 != points.end();)
		{
			Line2D l1(*(it - 1), *(it + 1));

			if (l1.pointDistance(*(it)) < 0.1)
				it = points.erase(it);
			else
				it++;
		}
	}
}

void MapObject::shadeTheWall(Color& color, const vector2D& wallLine, float shadePower)
{
	vector2D equator(Point(0.0, 0.0), Point(1.0, 0.0));

	auto angle = vector2D::angle(equator, wallLine);

	if (angle <= PI / 2) // NORTH
	{
		auto ratio = angle / (PI / 2) * shadePower + (1 - shadePower);

		color.red *= ratio;
		color.green *= ratio;
		color.blue *= ratio;
	}
	else // SOUTH
	{
		auto ratio = (angle - PI / 2) / (PI / 2);

		color.red = ratio * (1 - color.red) * shadePower + color.red;
		color.green = ratio * (1 - color.green) * shadePower + color.green;
		color.blue = ratio * (1 - color.blue) * shadePower + color.blue;
	}
}