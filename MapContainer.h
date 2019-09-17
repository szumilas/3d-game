#pragma once

#include <vector>
#include <memory>
#include "MapObject.h"

#include "Object3D.h"
#include "PointInsidePolygonDetector.h"
#include "Car.h"

class MapContainer
{
public:

	static void Init();
	static std::unique_ptr<MapContainer>& Instance();

	static void loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	static void displayWorld(std::pair<Point, Point>& camera);
	static void displayAllWorld();
	static void displaySector(const Point& point);
	static void displayBackground();
	static void displayLines();
	void addAIPoint(const Point& point);
	void removeAIPoints();
	void setAIPathActive() { AIPathActive = true; }
	bool getAIPathActive() { return AIPathActive; }
	static void SetFuturePoints(const int& futurePoint);
	static void ClearFuturePoints();
	void SaveAIPoints();
	void LoadAIPoints();
	static int GetNextPoint(const int& currentPoint) { return (currentPoint + 1) % AIPoints.size(); }
	static float GetNextPointDistance(const int& currentPoint) { return AIPointsDistances[currentPoint]; }

	static std::vector<std::vector<std::unique_ptr<MapObject>*>*> getCollidableObjectsInPosition(const Point& position);

private:

	static void displayAIPoints();
	static void recalculateAIPointsDistances();

public:

	static std::vector<Car> cars;
	static std::vector<std::pair<Point, bool>> AIPoints;

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

	static std::vector<float> AIPointsDistances;
	bool AIPathActive = false;

	static std::unique_ptr<MapContainer> _instance;

};