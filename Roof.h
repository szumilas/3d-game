#pragma once

#include "MapObject.h"
#include <set>
#include <queue>

class Roof : public MapObject
{

public:

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
	};

	struct Quadrangle
	{
		bool closed;
		long long idp1;
		long long idp2;
		Point p1;
		Point p2;
		Point p3;
		Point p4;
	};

	struct RoofSurface
	{
		std::vector<Point> points;
		std::vector<Point> texturePoints;
		Color color;
		unsigned int idTexture;
	};

	Roof(MapObject& mapObject);

	//void display();
	void calculateFinalGeometry();

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void calculateSpeedOfPoint(long long IdcurrentPoint, long long IdnextPoint, long long IdprevPoint);
	void calculateSpeedOfPoint(long long id);
	double lineCollapseTime(RoofPoint& p1, RoofPoint& p2);
	double triangleCollapseTime(RoofPoint& p1, RoofPoint& p2, RoofPoint& p3);
	void setCollisionPoint(long long& p1, long long& p2, long long& p3);
	long long oppositeTriangleWithEdge(long long idp1, long long idp2, long long idp3);
	void removeTriangle(long long idp1, long long idp2, long long idp3);
	void sortPoints(long long& idp1, long long& idp2, long long& idp3);
	void removeTriangle(long long idp1, long long idp2);
	void renamePointInTriangles(long long oldIdP, long long newIdP);
	void renamePointInSurfaces(long long oldIdP, long long newIdP);
	void removePointFromWavefront(long long id);
	double futureDistance(long long idp1, long long idp2, double h);
	void removeRoofPoint(long long id);
	int countTrianglesWithPoint(long long id);
	void removeBrokenTriangles();
	void removeEmptyWavefronts();
	void generateRoof(float roofStartLevel, float roofAngle);


	Point getRoofPoint(long long id)
	{
		for (int i = 0; i < roofPoints.size(); i++)
		{
			if (roofPoints[i].id == id)
				return roofPoints[i].point;
		}

		return Point();
	}

	RoofPoint getFullRoofPoint(long long id)
	{
		for (int i = 0; i < roofPoints.size(); i++)
		{
			if (roofPoints[i].id == id)
				return roofPoints[i];
		}

		return RoofPoint();
	}

	void updateRoofPoint(long long id, RoofPoint newPoint)
	{
		for (int i = 0; i < roofPoints.size(); i++)
		{
			if (roofPoints[i].id == id)
			{
				roofPoints[i] = newPoint;
				break;
			}
		}
	}

	void updatePoint(long long id, Point newPoint)
	{
		for (int i = 0; i < roofPoints.size(); i++)
		{
			if (roofPoints[i].id == id)
			{
				roofPoints[i].point = newPoint;
				break;
			}
		}
	}
	
	bool anyPointInTriangle(long long idA, long long idB, long long idC);
	long long nextRoofPointid();

	template <typename T>
	int nextIterator(long long id, std::vector<T>& container)
	{
		auto i = 0;
		for (; i < container.size(); i++)
		{
			if (container[i].id == id)
			{
				break;
			}
		}

		i += 1;
		i = i % container.size();
		return i;
	}

	template <>
	int nextIterator(long long id, std::vector<long long>& container)
	{
		auto i = 0;
		for (; i < container.size(); i++)
		{
			if (container[i] == id)
			{
				break;
			}
		}

		i += 1;
		i = i % container.size();
		return i;
	}


	void openWavefrontSurfaces();
	void closeWavefrontSurfaces();


	void openLongRoofLines();
	void createRoofLevelLongRoofLines();
	void closeLongRoofLine(const long long& id);
	void restartLongRoofPoint(const long long& id);
	void generateFlatRoof();

public:


private:

	float _roof_level;
	float gamma;


	std::vector<RoofPoint> roofPoints;
	std::list<std::tuple<Point, Point>> roofTriangleEdges;
	std::vector<Triangle> triangles;
	std::vector<std::vector<long long>> wavefront;
	std::vector<std::pair<Point, Point>> roofLines;

	std::vector<Quadrangle> surfaces;

	std::vector<std::tuple<Point, Point, long>> wavefrontLines;
	std::vector<std::tuple<long long, Point, Point>> longRoofLines;
	std::vector<Point> specialPoints;
	std::vector<std::pair<bool, std::vector<RoofSurface>>> roofsSurfaces;

	//BFS lgorithm:
	std::set<Point> graphIdsSet;
	std::vector<std::pair<long long, Point>> graphIds;
	std::vector<std::pair<long long, long long>> connections;

	
};