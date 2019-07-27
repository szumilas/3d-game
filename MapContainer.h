#pragma once

#include <vector>
#include <memory>
#include "MapObject.h"

#include "Object3D.h"
#include "PointInsidePolygonDetector.h"

class MapContainer
{
public:

	static void Init();
	static std::unique_ptr<MapContainer>& Instance();

	static void loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	static void displayWorld(Point& center, Point& lookAt);
	void displayAllWorld();

	static std::vector<std::unique_ptr<MapObject>*>& getCollidableObjectsInPosition(const Point& position);

private:


public:


private:

	std::vector<std::unique_ptr<Object3D>>* polygonsObjects = nullptr;

	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapObjectSections;
	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapCollidableObjectSections;
	static std::unique_ptr<MapObject>* background;

	std::vector<Object3D> myMap;

	static float deltaX;
	static float deltaY;

	static float maxX;
	static float minX;
	static float maxY;
	static float minY;

	vector2D equator{ Point{0,0}, Point{1,0} };


	static std::unique_ptr<MapContainer> _instance;

};