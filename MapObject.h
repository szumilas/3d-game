#pragma once

#include <iostream>
#include <map>

#include "Object3D.h"
#include "GlobalStructures.h"

#include "TextureManager.h"

class MapObject : public Object3D
{
public:

	MapObject() {};
	MapObject(long long id) { MapObject::id = id; };
	virtual void calculateXYfromRef(const std::map<long long, node> &nodes);
	virtual void calculateXYfromPoints(const std::vector<Point>& points);
	virtual void calculateFinalGeometry();
	void calculateBoundingCoordinates();
	virtual std::vector<std::pair<char*, char*>> getObjectXMLTags() { return {}; };
	virtual std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags() { return {}; }
	virtual void recalculateFinalGeometry() {}

	void applyKnownValues();

	long long getId() { return id; }
	Color getUniqueColorBasedOnId();
	void setId(long long newId) { id = newId; }
	void select();
	void deselect();

private:

	Color uniqueColorBasedOnId{-1, -1, -1};

protected:

	std::vector<Point> DouglasPeuckerAlgorithm(std::vector<Point>& l, float epsilon);
	void optimizePoints();
	void shadeTheWall(Color& color, const vector2D& wallLine, float shadePower);
	Color selectedColor{ 1.0f, 0.0f, 1.0f };
	void dividePointsPolygonIntoTriangles();
	void createBothRailsForSymmetryAxis(const std::vector<Point>& axisPoints, std::vector<Point>& finalLeftRail, std::vector<Point>& finalRightRail, float gauge);

	double epsilon = 0.1;
	float shadePower = 0.5;

public:
	//stats
	long long polygonsCount = 0;
	long long edgesCount = 0;

	std::vector<long long> refs;
	std::vector<Point> points;
	bool isSelected = false;
	bool isHidden = false;
	float maxX = -10000000.0f;
	float minX =  10000000.0f;
	float maxY = -10000000.0f;
	float minY =  10000000.0f;

	bool selectable = true;

private:

	long long id;

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
	std::string roof_shape;
	std::string _skip;
	std::string natural;
	std::string width;
	std::string type;
	std::string footway;
	std::string _custom_texture;
	std::string _custom_object;
	std::string _custom_area;


	float _height;
	float _min_height;
	float _width;
	Color _color;

	Color _colorCopy;

	Collidable collidable = Collidable::no;
};