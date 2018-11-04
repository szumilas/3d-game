#pragma once

#include "MapObject.h"

class Roof : public MapObject
{


	struct vertex
	{
		Point startPoint;
		Point edgePointStart;
		float dv;
		float dx;
		float dy;
		float beta;
	};


public:

	struct RoofData
	{
		Point startPoint;
		Point firstPoint;
		bool firstPointIsNull;
		Point secondPoint;
		Point directionPoint;
		float beta;

		bool operator==(const RoofData& second) { return startPoint == second.startPoint; }
	};

	struct RoofPoint
	{
		long long id;
		Point point;
		double dx;
		double dy;
	};

	struct Triangle
	{
		long long idp1;
		long long idp2;
		long long idp3;
		std::list<RoofPoint>::iterator p1;
		std::list<RoofPoint>::iterator p2;
		std::list<RoofPoint>::iterator p3;
	};

	Roof(MapObject& mapObject);

	void display();

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void calculateSpeedOfPoint(std::list<RoofPoint>::iterator& currentPoint, const std::list<RoofPoint>::iterator& nextPoint, const std::list<RoofPoint>::iterator& prevPoint);
	void calculateSpeedOfPoint(long long id);
	double lineCollapseTime(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2);
	double triangleCollapseTime(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3);
	void setCollisionPoint(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3);
	std::list<RoofPoint>::iterator oppositeTriangleWithEdge(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3);
	void removeTriangle(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3);
	void removeTriangle(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2);
	void renamePointInTriangles(std::list<RoofPoint>::iterator& oldP, std::list<RoofPoint>::iterator& newP);
	void removePointFromWavefront(long long id);
	double futureDistance(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, double h);
	void removeRoofPoint(long long id);
	void registerTriangles();


	double distanceIdId(long long id1, long long id2)
	{
		auto p1 = roofPoints.begin();
		auto p2 = roofPoints.begin();

		for (auto itRoofPoint = roofPoints.begin(); itRoofPoint != roofPoints.end(); itRoofPoint++)
		{
			if (itRoofPoint->id == id1)
				p1 = itRoofPoint;
			if (itRoofPoint->id == id2)
				p2 = itRoofPoint;
		}

		return p1->point.distance2D(p2->point);
	}

	template <typename T, typename Titerator>
	Titerator nextLoop(Titerator i, std::list<T> &container, int n = 1)
	{
		auto it = i;
		for (int q = 0; q < n; q++)
		{
			it = (std::next(it) != container.end()) ? std::next(it) : container.begin();
		}

		return it;
	}

	template <typename T, typename Titerator>
	Titerator prevLoop(Titerator i, std::list<T> &container, int n = 1)
	{
		auto it = i;
		for (int q = 0; q < n; q++)
		{
			it = (it != container.begin()) ? std::prev(it) : std::prev(container.end());
		}

		return it;
	}

	bool anyPointInTriangle(std::list<RoofPoint>::iterator& A, std::list<RoofPoint>::iterator& B, std::list<RoofPoint>::iterator& C);
	long long nextRoofPointid();

public:


private:

	std::vector<Point> specialPoints;
	std::vector<std::pair<Point, Point>> roofEdges;
	float _roof_level;

	std::list<RoofData> roofData;

	std::vector < std::tuple<Point, float, float, float>> specPoints;

	bool skip = false;
	float gamma;



	std::list<vertex> vertices;
	std::list<std::list<vertex>> verticiesList;
	std::list<std::tuple<Point, Point>> izolines;
	std::list<std::tuple<Point, Point>> ridges;

	std::list<RoofPoint> roofPoints;
	std::list<std::tuple<Point, Point>> roofTriangleEdges;
	std::list<Triangle> triangles;
	std::list<std::list<std::list<RoofPoint>::iterator>> wavefront;
	std::list<std::pair<Point, Point>> roofLines;
};