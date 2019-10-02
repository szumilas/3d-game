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
		e_AddPoint,
		e_RemovePoint,
		e_MovePoint,
		e_SelectPoint,
		e_SaveAIPoints,
		e_LoadAIPoints,
		e_AIPlay,
		e_AIPause,
		e_AIStop,
		e_AIStopAndRestartToSelectedPoint,
		e_StartRace,
		e_ConvertPathToAIPoints,
		e_ConvertAIPointsToPath,
		e_RemovePoints,
		e_AddSplinePoint,
		e_RemoveSplinePoint,
		e_MoveSplinePoint,
		e_SelectSplinePoint,
		e_RemoveSplinePoints,
		e_DivideSpline,
		e_IncreaseSplineSubpoints,
		e_DecreaseSplineSubpoints,
		e_ConvertSplineToCurrentPath,
		e_ConvertPathToRaceBarriers,
		e_ConvertPathToMeta,
		e_AddCameraPoint,
		e_RemoveCameraPoints,
		e_ConvertCameraPointsToSpline,
		e_PlayCameraSpline,
		e_ConvertCameraSplineToCarZero,
		e_PlayCameraSplineAroundCarZero,
		e_SaveCameraSpline,
	};


	struct PathStruct
	{
		Point center;
		Color color;
		bool selected;
	};

	struct SplineStruct
	{
		Spline spline;
		int selected = -1;
		float subpointsDistance = 0;
	};

	static void Init(int w, int h);
	static std::unique_ptr<MapContainer>& Instance();

	static void loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	static void MapContainer::addObjectsToSections(std::vector<std::unique_ptr<MapObject>>& mapObjects);
	static void displayWorld(std::pair<Point, Point>& camera);
	static void displayAllWorld();
	static void displaySector(const Point& point);
	static void displayBackground();
	static void displayLines();
	static void displayMapEditorPanel();
	static void displayCounter();
	static void pickTool(float pX, float pY);
	static void useTool(const Point& point);

	void addPoint(const Point& point);
	void removePoint(const Point& point = Point());
	void selectPoint(const Point& point);
	void AIStop(const Point& point = Point());
	void AIStopAndRestartToSelectedPoint(const Point& point = Point());
	void startRace(const Point& point = Point());
	void ConvertPathToAIPoints(const Point& point = Point());
	void ConvertAIPointsToPath(const Point& point = Point());
	void AddSplinePoint(const Point& point);
	void RemoveSplinePoint(const Point& point = Point());
	void MoveSplinePoint(const Point& point);
	void SelectSplinePoint(const Point& point);
	void RemoveSplinePoints(const Point& point = Point());
	void DivideSpline(const Point& point = Point());
	void IncreaseSplineSubpoints(const Point& point = Point());
	void DecreaseSplineSubpoints(const Point& point = Point());
	void ConvertSplineToCurrentPath(const Point& point = Point());
	void ConvertPathToRaceBarriers(const Point& point = Point());
	void ConvertPathToMeta(const Point& point = Point());
	void AddCameraPoint(const Point& point = Point());
	void RemoveCameraPoints(const Point& point = Point());
	void ConvertCameraPointsToSpline(const Point& point = Point());
	void PlayCameraSpline(const Point& point = Point());
	void ConvertCameraSplineToCarZero(const Point& point = Point());
	void PlayCameraSplineAroundCarZero(const Point& point = Point());
	void SaveCameraSpline(const Point& point = Point());

	void createRaceObjects();
	static std::vector<PathStruct> generateSubsplinePath(bool keepOriginalPoints = false);
	void resetCarPositionsToPoint(int idPoint);
	void movePoint(const Point& point);
	void removePoints(const Point& point = Point());
	void AIPlay(const Point& point = Point());
	bool getAIPathActive() { return AIPathActive; }
	void AIPause(const Point& point = Point());
	static void SetFuturePoints(const int& futurePoint);
	static void ClearFuturePoints();
	void SaveAIPoints(const Point& point = Point());
	void LoadAIPoints(const Point& point = Point());
	static std::vector<std::vector<int>> createTools();
	static std::map<int, void (MapContainer::*)(const Point&)> createToolsMap();
	static int GetNextPoint(const int& currentPoint) { return (currentPoint + 1) % AIPoints.size(); }
	static float GetNextPointDistance(const int& currentPoint) { return AIPointsDistances[currentPoint]; }
	int getSelectedPointIndex();
	static void initCars();
	static void initRaceTimer();
	static void updateRaceTimer();
	static void displayRaceTimer();
	void LoadRaceStartCameraPoints();
	void introFinished() { raceTimer.state = RaceTimer::State::Red4; }

	static std::vector<std::vector<std::unique_ptr<MapObject>*>*> getCollidableObjectsInPosition(const Point& position);

private:

	static void displayPath(const std::vector<PathStruct>& path, const Color& color);
	static void displayMapEditorPoints();
	static void displayAIPoints();
	static void displayCurrentPath();
	static void displayCurrentSpline();
	static void recalculateAIPointsDistances();

public:

	static std::vector<Car> cars;
	static std::vector<PathStruct> currentPath;
	static std::vector<PathStruct> AIPoints;
	static std::vector<std::pair<Point, Point>> raceBarriers;
	static std::pair<Point, Point> meta;
	static SplineStruct currentSpline;
	static std::pair<Spline, Spline> cameraSpline;

	static RaceTimer raceTimer;

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

	static Color AIPointsColor;
	static Color pointsColor;
	static Color splinePointsColor;
	static Color splineSubointsColor;

	static std::unique_ptr<MapContainer> _instance;

};