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

	Roof(MapObject& mapObject);

	void display();

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
	//void registerTriangles();


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

	template <typename T>
	int prevIterator(long long id, std::vector<T>& container)
	{
		auto i = 0;
		for (; i < container.size(); i++)
		{
			if (container[i].id == id)
			{
				break;
			}
		}

		i -= 1;
		i = i % container.size();
		return i;
	}

	template <>
	int prevIterator(long long id, std::vector<long long>& container)
	{
		auto i = 0;
		for (; i < container.size(); i++)
		{
			if (container[i] == id)
			{
				break;
			}
		}

		i -= 1;
		i = (i + container.size()) % container.size();
		return i;
	}

	void openWavefrontSurfaces();
	void closeWavefrontSurfaces();



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

	std::vector<RoofPoint> roofPoints;
	std::list<std::tuple<Point, Point>> roofTriangleEdges;
	std::vector<Triangle> triangles;
	std::vector<std::vector<long long>> wavefront;
	std::vector<std::pair<Point, Point>> roofLines;

	std::vector<Quadrangle> surfaces;

	std::vector<std::tuple<Point, Point, long>> wavefrontLines;

	
};