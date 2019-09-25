#pragma once

#include <vector>
#include <memory>
#include "MapObject.h"

#include "Object3D.h"
#include "PointInsidePolygonDetector.h"
#include "Car.h"
#include "RaceTimer.h"

class MapContainer
{
public:

	enum Tools
	{
		AddPoint,
		RemovePoint,
		MovePoint,
		SelectPoint,
		SaveAIPath,
		LoadAIPath,
		AIPlay,
		AIPause,
		AIStop,
		AIStopAndResumePosition,
		StartRace,
	};


	struct AIPointStruct
	{
		Point center;
		Color color;
		bool selected;


	};

	static void Init(int w, int h);
	static std::unique_ptr<MapContainer>& Instance();

	static void loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	static void displayWorld(std::pair<Point, Point>& camera);
	static void displayAllWorld();
	static void displaySector(const Point& point);
	static void displayBackground();
	static void displayLines();
	static void displayMapEditorPanel();
	static void pickTool(float pX, float pY);
	static void useTool(const Point& point);
	void addAIPoint(const Point& point);
	void removeAIPoint(const Point& point = Point());
	void selectAIPoint(const Point& point);
	void stopAllCars(const Point& point = Point());
	void stopAllCarsToSelectedPoint(const Point& point = Point());
	void startRace(const Point& point = Point());
	void resetCarPositionsToPoint(int idPoint);
	void moveAIPoint(const Point& point);
	void removeAIPoints();
	void setAIPathActive(const Point& point = Point()) { AIPathActive = true; raceTimer.startRace(); }
	bool getAIPathActive() { return AIPathActive; }
	void pauseAllCars(const Point& point = Point());
	static void SetFuturePoints(const int& futurePoint);
	static void ClearFuturePoints();
	void SaveAIPoints(const Point& point = Point());
	void LoadAIPoints(const Point& point = Point());
	static std::vector<std::vector<int>> createTools();
	static std::map<int, void (MapContainer::*)(const Point&)> createToolsMap();
	static int GetNextPoint(const int& currentPoint) { return (currentPoint + 1) % AIPoints.size(); }
	static float GetNextPointDistance(const int& currentPoint) { return AIPointsDistances[currentPoint]; }
	int getSelectedPointIndex();
	static void initRaceTimer();
	static void updateRaceTimer();
	static void displayRaceTimer();

	static std::vector<std::vector<std::unique_ptr<MapObject>*>*> getCollidableObjectsInPosition(const Point& position);

private:

	static void displayAIPoints();
	static void recalculateAIPointsDistances();

public:

	static std::vector<Car> cars;
	static std::vector<AIPointStruct> AIPoints;

private:

	static int w;
	static int h;

	static int mapEditorSelectedPanel;
	static int mapEditorSelectedTool;
	static int currentToolId;

	static std::vector<std::vector<int>> tools;
	static std::map<int, void (MapContainer::*)(const Point&)> toolsMap;

	std::vector<std::unique_ptr<Object3D>>* polygonsObjects = nullptr;

	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapObjectSections;
	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapCollidableObjectSections;
	static std::unique_ptr<MapObject>* background;

	static RaceTimer raceTimer;


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