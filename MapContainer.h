#pragma once

#include <vector>
#include <memory>
#include "MapObject.h"

#include "Object3D.h"
#include "PointInsidePolygonDetector.h"
#include "Car.h"
#include "RaceTimer.h"
#include "Track.h"

class MapContainer
{
	friend class MapManager;

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
		e_SaveNewObject,
		e_DeleteLastNewObject,
		e_AddTree,
		e_AddStreetLamp,
		e_AddBuilding,
		e_AddBarrier,
		e_AddRamp,
		e_NewObjectAddHeight,
		e_NewObjectReduceHeight,
		e_NewObjectAddMinHeight,
		e_NewObjectReduceMinHeight,
		e_NewObjectAddRed,
		e_NewObjectReduceRed,
		e_NewObjectAddGreen,
		e_NewObjectReduceGreen,
		e_NewObjectAddBlue,
		e_NewObjectReduceBlue,
		e_NewObjectAddRemoveCustomTexture,
		e_NewObjectNextTextrue,

		e_ReloadMap,
		e_SaveMapObject,
		e_EditMapObject,
		e_EditMapObjectNextBuildingTexture,
		e_EditMapObjectNextRoofTexture,
		e_EditMapObjectDoNotGenerateBottomRoof,
		e_EditMapObjectChangeRoofType,
		e_EditMapObjectAddHeight,
		e_EditMapObjectReduceHeight,
		e_EditMapObjectAddRed,
		e_EditMapObjectReduceRed,
		e_EditMapObjectAddGreen,
		e_EditMapObjectReduceGreen,
		e_EditMapObjectAddBlue,
		e_EditMapObjectReduceBlue,
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
	static long long shootRay(std::vector<std::pair<int, int>>& sectionsToDisplay, Point& center, vector2D& currentRay);
	static void checkMiddleRay(vector2D& firstRay, long long firstRayCollisionId, vector2D& lastRay, long long lastRayCollisionId, std::vector<std::pair<int, int>>& sectionsToDisplay, Point& center);
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
	void SaveNewObject(const Point& point = Point());
	void DeleteLastNewObject(const Point& point = Point());
	void AddTree(const Point& point = Point());
	void AddStreetLamp(const Point& point = Point());
	void AddBuilding(const Point& point = Point());
	void AddBarrier(const Point& point = Point());
	void AddRamp(const Point& point = Point());
	void NewObjectAddHeight(const Point& point = Point());
	void NewObjectReduceHeight(const Point& point = Point());
	void NewObjectAddMinHeight(const Point& point = Point());
	void NewObjectReduceMinHeight(const Point& point = Point());
	void NewObjectAddRed(const Point& point = Point());
	void NewObjectReduceRed(const Point& point = Point());
	void NewObjectAddGreen(const Point& point = Point());
	void NewObjectReduceGreen(const Point& point = Point());
	void NewObjectAddBlue(const Point& point = Point());
	void NewObjectReduceBlue(const Point& point = Point());
	void NewObjectAddRemoveCustomTexture(const Point& point = Point());
	void NewObjectNextTexture(const Point& point = Point());

	void ReloadMap(const Point& point = Point());
	void SaveMapObject(const Point& point = Point());
	void EditMapObject(const Point& point = Point());
	void EditMapObjectNextBuildingTexture(const Point& point = Point());
	void EditMapObjectNextRoofTexture(const Point& point = Point());
	void EditMapObjectDoNotGenerateBottomRoof(const Point& point = Point());
	void EditMapObjectChangeRoofType(const Point& point = Point());
	void EditMapObjectAddHeight(const Point& point = Point());
	void EditMapObjectReduceHeight(const Point& point = Point());
	void EditMapObjectAddRed(const Point& point = Point());
	void EditMapObjectReduceRed(const Point& point = Point());
	void EditMapObjectAddGreen(const Point& point = Point());
	void EditMapObjectReduceGreen(const Point& point = Point());
	void EditMapObjectAddBlue(const Point& point = Point());
	void EditMapObjectReduceBlue(const Point& point = Point());


	void setRaceDetails(CarBrand selectedCar, TrackName selectedTrack, int noOfLaps, int noOfOponents);
	void setFreeRide(CarBrand selectedCar, Point position);

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
	void LoadAIPointsFromTrackName(TrackName selectedTrack);
	static std::vector<std::vector<int>> createTools();
	static std::map<int, void (MapContainer::*)(const Point&)> createToolsMap();
	static int GetNextPoint(const int& currentPoint) { return (currentPoint + 1) % AIPoints.size(); }
	static float GetNextPointDistance(const int& currentPoint) { return AIPointsDistances[currentPoint]; }
	int getSelectedPointIndex();
	static void initCars();
	static void initCars(CarBrand selectedCar, int noOfOponents);
	static void resetCameraViews();
	static void initRaceTimer();
	static void deactivateRaceTimer() { raceTimer.active = false; }
	static void activateRaceTimer() { raceTimer.active = true; }
	static void updateRaceTimer();
	static void displayRaceTimer();
	void LoadRaceStartCameraPoints();
	void introFinished() { raceTimer.state = RaceTimer::State::Red4; }
	bool raceActive() { return raceTimer.state == RaceTimer::State::Inactive; }
	bool timerActive() { return raceTimer.state != RaceTimer::State::Intro && raceTimer.state != RaceTimer::State::Outro; }

	static std::vector<std::vector<std::unique_ptr<MapObject>*>*> getCollidableObjectsInPosition(const Point& position);

private:

	struct BuildingEdited
	{
		MapObject* object = nullptr;

		bool heightChanged = false;
		bool textureChanged = false;
		bool colorChanged = false;
	};

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
	static int lapRecord;
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
	BuildingEdited buildingEdited;

	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapObjectSections;
	static std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> mapCollidableObjectSections;
	static std::vector<std::unique_ptr<MapObject>*> highBuildingSection;
	static std::unique_ptr<MapObject>* background;
	static std::vector<std::unique_ptr<MapObject>> extraObjects;


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