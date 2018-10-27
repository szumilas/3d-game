#pragma once

#include <map>

#include "Object3D.h"
#include "GlobalStructures.h"

class MapObject : public Object3D
{
public:

	virtual void display();
	virtual void calculateXYfromRef(const std::map<long long, node> &nodes);

	void applyKnownValues();

private:


public:

	std::vector<long long> refs;
	std::vector<Point> points;

private:

	std::map<std::string, std::tuple<float, float, float>> colorMap
	{
		{"white", {1.0f, 1.0f, 1.0f}},
		{"black", {0.0f, 0.0f, 0.0f}},
		{"red", {1.0f, 0.0f, 0.0f}},
		{"green", {0.0f, 1.0f, 0.0f}},
		{"blue", {0.0f, 0.0f, 1.0f}},
		{"grey", {0.5f, 0.5f, 0.5f}},
	};

public:
	/*TAGS*/
	std::string area_highway;
	std::string building;
	std::string landuse;
	std::string waterway;
	std::string height;
	std::string building_part;
	std::string min_height;
	std::string barrier;
	std::string leisure;
	std::string amenity;
	std::string highway;
	std::string man_made;
	std::string railway;
	std::string colour;


	float _height;
	float _min_height;
	float _red;
	float _green;
	float _blue;
};