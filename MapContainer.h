#pragma once

#include <vector>
#include <memory>
#include "MapObject.h"

#include "Object3D.h"
#include "PointInsidePolygonDetector.h"

class MapContainer
{

public:

	void loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	void displayWorld(Point& center, Point& lookAt);

private:


public:


private:

	std::vector<std::unique_ptr<Object3D>>* polygonsObjects = nullptr;

	std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapObjectSections;
	std::unique_ptr<MapObject>* background;

	std::vector<Object3D> myMap;

	float deltaX;
	float deltaY;

	float maxX;
	float minX;
	float maxY;
	float minY;

	vector2D equator{ Point{0,0}, Point{1,0} };

};