#pragma once

#include "Object3D.h"
#include "GlobalStructures.h"

class MapObject : public Object3D
{
public:

	virtual void display();

private:


public:

	std::vector<long long> refs;
	std::vector<Point> points;

private:


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


	float _height;
	float _min_height;
};