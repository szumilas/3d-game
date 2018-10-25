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
};