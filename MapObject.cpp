#include "MapObject.h"

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

void MapObject::calculateFinalGeometry(TextureManager* textureManager)
{

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
	for (auto& polygon : polygons)
	{
		std::swap(polygon.color, polygon.additionalColor);
	}

	std::cout << id << " ";
}

void MapObject::deselect()
{
	isSelected = false;
	for (auto& polygon : polygons)
	{
		std::swap(polygon.color, polygon.additionalColor);
	}
}

void MapObject::optimizePoints()
{
	/*if (points.size() > 2)
	{
		for (auto it = points.begin() + 1; it + 2 != points.end();)
		{
			Line2D l1(*(it - 1), *(it + 1));

			if (l1.pointDistance(*(it)) < 0.1)
				it = points.erase(it);
			else
				it++;
		}
	}*/

	std::vector<Point> l1;
	std::vector<Point> l2;

	int it1 = 0;
	int it2 = 0;

	double longestDistance = 0.0;

	for (int q = 0; q < points.size() - 1; q++)
	{
		for (int w = 1; w < points.size() - 1; w++)
		{
			if (points[q].distance2D(points[w]) > longestDistance)
			{
				longestDistance = points[q].distance2D(points[w]);
				it1 = q;
				it2 = w;
			}
		}
	}

	for (int q = it1; ; q = (q + 1) % (points.size() - 1))
	{
		l1.push_back(points[q]);
		if (q == it2)
			break;
	}
	for (int q = it2; ; q = (q + 1) % (points.size() - 1))
	{
		l2.push_back(points[q]);
		if (q == it1)
			break;
	}


	double epsilon = 0.1;

	auto optimizedPoints1 = DouglasPeuckerAlgorithm(l1, epsilon);
	auto optimizedPoints2 = DouglasPeuckerAlgorithm(l2, epsilon);

	points.clear();
	points.reserve(optimizedPoints1.size() + optimizedPoints2.size() - 1);
	points.insert(points.end(), optimizedPoints1.begin(), optimizedPoints1.end() - 1);
	points.insert(points.end(), optimizedPoints2.begin(), optimizedPoints2.end());
}

std::vector<Point> MapObject::DouglasPeuckerAlgorithm(std::vector<Point>& PointList, float epsilon)
{
	// Find the point with the maximum distance
	double dmax = 0.0;
	int index = 0;
	int end = PointList.size();
	for (int q = 1; q < end - 1; q++)
	{
		double d = Line2D(PointList[0], PointList.back()).pointDistance(PointList[q]);

		if (d > dmax)
		{
			index = q;
			dmax = d;
		}
	}

	std::vector<Point> res;

	// If max distance is greater than epsilon, recursively simplify
	if (dmax > epsilon)
	{
		// Recursive call
		std::vector<Point> l1(PointList.begin(), PointList.begin() + index + 1);
		std::vector<Point> l2(PointList.begin() + index, PointList.end());

		auto vec1 = DouglasPeuckerAlgorithm(l1, epsilon);
		auto vec2 = DouglasPeuckerAlgorithm(l2, epsilon);

		// Build the result list
		//ResultList[] = { recResults1[1...length(recResults1) - 1], recResults2[1...length(recResults2)] }
		res.reserve(vec1.size() + vec2.size() - 1); // preallocate memory
		res.insert(res.end(), vec1.begin(), vec1.end() - 1);
		res.insert(res.end(), vec2.begin(), vec2.end());
	}
	else
	{
		//ResultList[] = { PointList[1], PointList[end] }
		res.push_back(PointList[0]);
		res.push_back(PointList.back());
	}
	// Return the result
	return res;
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