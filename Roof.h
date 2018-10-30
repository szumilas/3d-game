#pragma once

#include "MapObject.h"

class Roof : public MapObject
{
public:

	struct RoofData
	{
		Point startPoint;
		Point firstPoint;
		bool firstPointIsNull;
		Point secondPoint;
		Point directionPoint;
		float beta;

		bool operator==(const RoofData& second) { return startPoint == second.startPoint; }
	};

	Roof(MapObject& mapObject);

	void display();

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);

public:


private:

	std::vector<Point> specialPoints;
	std::vector<std::pair<Point, Point>> roofEdges;
	float _roof_level;

	std::list<RoofData> roofData;

	std::vector < std::tuple<Point, float, float, float>> specPoints;

	bool skip = false;
};