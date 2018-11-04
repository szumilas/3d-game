#include "Roof.h"

#include "PointInsidePolygonDetector.h"




double measureAngle(Point& currentPoint, Point& nextPoint, Point& previousPoint)
{
	double lenA = currentPoint.distance2D(nextPoint);
	double lenB = currentPoint.distance2D(previousPoint);
	double lenC = nextPoint.distance2D(previousPoint);

	return acos((lenA * lenA + lenB * lenB - lenC * lenC) / (2 * lenA * lenB));
}



Roof::Roof(MapObject& mapObject) : MapObject(mapObject)
{
	if (!_height)
	{
		_roof_level = 15.0f;
	}
	_red = 0.7f;
	_green = 0.7f;
	_blue = 0.7f;

};

void Roof::display()
{
	/*glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(_red, _green, _blue);
	for (size_t q = 0; q < points.size() - 1; q++)
	{
		//auto point = points[q];
		//auto nextPoint = points[q + 1];

		glVertex3f(points[q].x, points[q].y, _roof_level);

	}
	glEnd();*/
	glLineWidth(1.0f);

	glLineWidth(2.0f);
	glColor3f(0, 1.0f, 0);

	/*glBegin(GL_LINES);
	glVertex3f(points[0].x, points[0].y, _roof_level);
	glVertex3f(points[0].x, points[0].y, _roof_level + 5);
	glEnd();*/

	/*
	for (size_t q = 0, limit = specialPoints.size() - 1; q < limit; q+=2)
	{
	glBegin(GL_LINES);
	glVertex3f(specialPoints[q].x, specialPoints[q].y, specialPoints[q].z);
	glVertex3f(specialPoints[q + 1].x, specialPoints[q + 1].y, specialPoints[q + 1].z);
	glEnd();
	}*/

	/*glLineWidth(2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (size_t q = 0, limit = roofEdges.size(); q < limit; q++)
	{
		glBegin(GL_LINES);
		glVertex3f(roofEdges[q].first.x, roofEdges[q].first.y, roofEdges[q].first.z);
		glVertex3f(roofEdges[q].second.x, roofEdges[q].second.y, roofEdges[q].second.z);
		glEnd();
	}
	glLineWidth(1.0f);*/

	/*glLineWidth(2.0f);
	glColor3f(0.5f, 0.0f, 0.0f);
	for (auto i = izolines.begin(); i != izolines.end(); i++)
	{
		glBegin(GL_LINES);

		glVertex3f(std::get<0>(*i).x, std::get<0>(*i).y, std::get<0>(*i).z);
		glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, std::get<1>(*i).z);
		glEnd();
	}
	glLineWidth(1.0f);*/

	/*glLineWidth(2.0f);
	glColor3f(0.5f, 1.0f, 0.0f);
	for (auto i = ridges.begin(); i != ridges.end(); i++)
	{
		glBegin(GL_LINES);

		glVertex3f(std::get<0>(*i).x, std::get<0>(*i).y, std::get<0>(*i).z);
		glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, std::get<1>(*i).z);
		glEnd();
	}
	glLineWidth(1.0f);*/


	glPointSize(8.0f);

	glBegin(GL_POINTS);

	for (size_t q = 0, limit = specPoints.size(); q < limit; q++)
	{
		glColor3f(std::get<1>(specPoints[q]), std::get<2>(specPoints[q]), std::get<3>(specPoints[q]));
		glVertex3f(std::get<0>(specPoints[q]).x, std::get<0>(specPoints[q]).y, std::get<0>(specPoints[q]).z);
	}
	glEnd();
	glPointSize(1.0f);



	/*glColor3f(0.5f, 1.0f, 0.0f);
	for (auto i = roofTriangleEdges.begin(); i != roofTriangleEdges.end(); i++)
	{
		glBegin(GL_LINES);
		glVertex3f(std::get<0>(*i).x, std::get<0>(*i).y, _roof_level);
		glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, _roof_level);
		glEnd();
	}*/



	glLineWidth(1.0f);
	for (auto i = triangles.begin(); i != triangles.end(); i++)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(0.5f, 1.0f, 0.0f);
		glVertex3f((*i).p1->point.x, (*i).p1->point.y, (*i).p1->point.z);
		glVertex3f((*i).p2->point.x, (*i).p2->point.y, (*i).p2->point.z);
		glVertex3f((*i).p3->point.x, (*i).p3->point.y, (*i).p3->point.z);
		glEnd();
	}

	/*glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (auto il = wavefront.begin(); il != wavefront.end(); il++)
	{
		for (auto i = (*il).begin(); i != (*il).end(); i++)
		{
			glVertex3f((*i)->point.x, (*i)->point.y, (*i)->point.z);
		}
	}
	glEnd();*/

	for (auto i = roofPoints.begin(); i != roofPoints.end(); i++)
	{
		Point p(i->point.x + i->dx, i->point.y + i->dy, i->point.z);
		glBegin(GL_LINES);
		glColor3f(0.2f, 0.2f, 1.0f);
		glVertex3f(i->point.x, i->point.y, i->point.z);
		glVertex3f(p.x, p.y, p.z);
		glEnd();
	}

	for (auto i = roofLines.begin(); i != roofLines.end(); i++)
	{
		glBegin(GL_LINES);
		glColor3f(0.8f, 0.2f, 0.4f);
		glVertex3f(i->first.x, i->first.y, i->first.z);
		glVertex3f(i->second.x, i->second.y, i->second.z);
		glEnd();
	}

	/*glPointSize(2.0f);
	glBegin(GL_POINTS);
	glColor3f(0.5f, 1.0f, 0.5f);
	for (size_t q = 0, limit = points.size(); q < limit; q++)
	{
		glVertex3f(points[q].x, points[q].y, _roof_level);
	}
	glEnd();
	glPointSize(1.0f);*/

}

bool positiveAngle(vector2D& v1, vector2D& v2)
{
	return v1.x * v2.y - v1.y * v2.x >= 0;
}

bool Roof::anyPointInTriangle(std::list<RoofPoint>::iterator& A, std::list<RoofPoint>::iterator& B, std::list<RoofPoint>::iterator& C)
{
	PointInsidePolygonDetector pointInsidePolygonDetector;

	std::vector<Point> triangle;
	triangle.push_back(A->point);
	triangle.push_back(B->point);
	triangle.push_back(C->point);

	for (auto it = nextLoop(C, roofPoints); it != A; it = nextLoop(it, roofPoints))
	{
		bool result = pointInsidePolygonDetector.isInside(triangle, it->point);
		if (result)
			return true;
	}

	return false;
}


void Roof::calculateSpeedOfPoint(long long id)
{
	auto currentWavefront = wavefront.begin();
	auto pointOnWavefront = currentWavefront->begin();
	for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
	{
		for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
		{
			if ((*itWavefront)->id == id)
			{
				currentWavefront = itWavefrontList;
				pointOnWavefront = itWavefront;
				goto wavefrontFound4;
			}
		}
	}
	wavefrontFound4:

	auto nextPointOnWavefront = nextLoop(pointOnWavefront, *currentWavefront);
	auto previousPointOnWavefront = prevLoop(pointOnWavefront, *currentWavefront);

	calculateSpeedOfPoint(*pointOnWavefront, *nextPointOnWavefront, *previousPointOnWavefront);
}




void Roof::calculateSpeedOfPoint(std::list<RoofPoint>::iterator& currentPoint, const std::list<RoofPoint>::iterator& nextPoint, const std::list<RoofPoint>::iterator& prevPoint)
{
	vector2D vnext(currentPoint->point, nextPoint->point);
	vector2D vprev(currentPoint->point, prevPoint->point);

	vnext.convertIntoUnitVector();
	vprev.convertIntoUnitVector();

	vector2D vbisection(vnext, vprev);
	vbisection.convertIntoUnitVector();

	double alpha = measureAngle(currentPoint->point, prevPoint->point, nextPoint->point) / 2;
	double beta = atan(tan(gamma) * sin(alpha));

	if (vector2D::crossProduct(vbisection, vnext) < 0)
	{
		vbisection.negate();
	}

	vbisection.x /= tan(beta);
	vbisection.y /= tan(beta);

	currentPoint->dx = vbisection.x;
	currentPoint->dy = vbisection.y;
}

double Roof::lineCollapseTime(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2)
{
	double a = p1->point.x;
	double b = p1->dx;
	double c = p1->point.y;
	double d = p1->dy;
	double e = p2->point.x;
	double f = p2->dx;
	double g = p2->point.y;
	double h = p2->dy;

	double sqrtValue = (pow(2 * a * b - 2 * a * f - 2 * b * e + 2 * c * d - 2 * c * h - 2 * d * g + 2 * e * f + 2 * g * h, 2) - 4 * (a * a - 2 * a * e + c * c - 2 * c * g + e * e + g * g) * (b * b - 2 * b * f + d * d - 2 * d * h + f * f + h * h));
	if (sqrtValue < 0.0 && sqrtValue > -0.0001)
		sqrtValue = 0.0;

	return (sqrt(sqrtValue) - 2 * a * b + 2 * a * f + 2 * b * e - 2 * c * d + 2 * c * h + 2 * d * g - 2 * e * f - 2 * g * h) / (2 * (b *b - 2 * b * f + d * d - 2 * d * h + f * f + h * h));
}

double Roof::triangleCollapseTime(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3)
{
	double a = p1->point.x;
	double b = p1->dx;
	double c = p1->point.y;
	double d = p1->dy;
	double e = p2->point.x;
	double f = p2->dx;
	double g = p2->point.y;
	double h = p2->dy;
	double i = p3->point.x;
	double j = p3->dx;
	double k = p3->point.y;
	double l = p3->dy;

	double s1 = ((a*(d - h) - a*(d - l) - c*(b - f) + c*(b - j) - k*(f - b) - g*(b - j) + e*(d - l) - i*(d - h)) + sqrt(pow(-a*(d - h) + a*(d - l) + c*(b - f) - c*(b - j) + k*(f - b) + g*(b - j) - e*(d - l) + i*(d - h), 2) - 4 * (a * g - a * k - c * e + c * i + e * k - g * i) * ((b - f) * (d - l) + (b - j) * (h - d)))) / (2 * ((b - f) * (d - l) + (b - j) * (h - d)));
	double s2 = ((a*(d - h) - a*(d - l) - c*(b - f) + c*(b - j) - k*(f - b) - g*(b - j) + e*(d - l) - i*(d - h)) - sqrt(pow(-a*(d - h) + a*(d - l) + c*(b - f) - c*(b - j) + k*(f - b) + g*(b - j) - e*(d - l) + i*(d - h), 2) - 4 * (a * g - a * k - c * e + c * i + e * k - g * i) * ((b - f) * (d - l) + (b - j) * (h - d)))) / (2 * ((b - f) * (d - l) + (b - j) * (h - d)));

	if (s1 > 0 && s2 > 0)
		return std::min(s1, s2);
	else
		return std::max(s1, s2);
}


void Roof::setCollisionPoint(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3)
{
	double dist12 = p1->point.distance2D(p2->point);
	double dist13 = p1->point.distance2D(p3->point);
	double dist23 = p2->point.distance2D(p3->point);

	double max = std::max(std::max(dist12, dist13), dist23);

	if (max == dist12)
	{
		swap(p1, p3);
		swap(p2, p3);
	}
	else if (max == dist13)
	{
		swap(p1, p2);
		swap(p2, p3);
	}
}

std::list<Roof::RoofPoint>::iterator Roof::oppositeTriangleWithEdge(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3)
{
	//edge p1-p2
	//triangle not containing p3
	for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		if ((p1 == itTriangle->p1 || p1 == itTriangle->p2 || p1 == itTriangle->p3) && 
			(p2 == itTriangle->p1 || p2 == itTriangle->p2 || p2 == itTriangle->p3))
		{
			if (!(p3 == itTriangle->p1 || p3 == itTriangle->p2 || p3 == itTriangle->p3))
			{
				if (itTriangle->p1 != p1 && itTriangle->p1 != p2)
					return itTriangle->p1;
				if (itTriangle->p2 != p1 && itTriangle->p2 != p2)
					return itTriangle->p2;
				if (itTriangle->p3 != p1 && itTriangle->p3 != p2)
					return itTriangle->p3;
			}
		}
	}

	return roofPoints.end();
}

void Roof::removeTriangle(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, std::list<RoofPoint>::iterator& p3)
{
	for (std::list<Triangle>::iterator itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		if ((p1 == itTriangle->p1 || p1 == itTriangle->p2 || p1 == itTriangle->p3) &&
			(p2 == itTriangle->p1 || p2 == itTriangle->p2 || p2 == itTriangle->p3) &&
			(p3 == itTriangle->p1 || p3 == itTriangle->p2 || p3 == itTriangle->p3))
		{
			itTriangle = triangles.erase(itTriangle);
			break;
		}
	}
}

void Roof::removeTriangle(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2)
{
	for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		if ((p1 == itTriangle->p1 || p1 == itTriangle->p2 || p1 == itTriangle->p3) &&
			(p2 == itTriangle->p1 || p2 == itTriangle->p2 || p2 == itTriangle->p3))
		{
			triangles.erase(itTriangle);
			break;
		}
	}
}

void Roof::renamePointInTriangles(std::list<RoofPoint>::iterator& oldP, std::list<RoofPoint>::iterator& newP)
{
	for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		if (oldP == itTriangle->p1)
		{
			itTriangle->p1 = newP;
			itTriangle->idp1 = newP->id;
		}
		if (oldP == itTriangle->p2)
		{
			itTriangle->p2 = newP;
			itTriangle->idp2 = newP->id;
		}
		if (oldP == itTriangle->p3)
		{
			itTriangle->p3 = newP;
			itTriangle->idp3 = newP->id;
		}
	}
}

void Roof::removePointFromWavefront(long long id)
{
	for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
	{
		for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
		{
			if ((*itWavefront)->id == id)
			{
				(*itWavefrontList).erase(itWavefront);
				break;
			}
		}
	}
}

double Roof::futureDistance(std::list<RoofPoint>::iterator& p1, std::list<RoofPoint>::iterator& p2, double h)
{
	auto futureP1 = p1->point;
	auto futureP2 = p2->point;

	futureP1.x += p1->dx * h;
	futureP1.y += p1->dy * h;
	futureP2.x += p2->dx * h;
	futureP2.y += p2->dy * h;

	return futureP1.distance2D(futureP2);
}

long long Roof::nextRoofPointid()
{
	long long maxId = -1;
	
	for (auto it = roofPoints.begin(); it != roofPoints.end(); it++)
	{
		if (it->id > maxId)
			maxId = it->id;
	}

	return maxId + 1;
}

void Roof::removeRoofPoint(long long id)
{
	for (auto it = roofPoints.begin(); it != roofPoints.end(); it++)
	{
		if (it->id == id)
		{
			roofPoints.erase(it);
			break;
		}
	}
}

void Roof::registerTriangles()
{
	for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
		{
			for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
			{
				if ((*itWavefront)->id == itTriangle->idp1)
				{
					itTriangle->p1 = *itWavefront;
				}
				if ((*itWavefront)->id == itTriangle->idp2)
				{
					itTriangle->p2 = *itWavefront;
				}
				if ((*itWavefront)->id == itTriangle->idp3)
				{
					itTriangle->p3 = *itWavefront;
				}
			}
		}
	}

}

void Roof::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	gamma = 30 / 180.0 * 3.14;


	for (size_t q = 0; q < points.size() - 1; q++)
	{
		RoofPoint roofPoint;
		roofPoint.id = q;
		roofPoint.point = points[q];
		roofPoint.point.z = _roof_level;

		roofPoints.push_back(roofPoint);
	}

	auto roofPointsCopy = roofPoints;

	auto firstPoint = std::next(roofPoints.begin());
	auto secondPoint = roofPoints.begin();
	auto thirdPoint = roofPoints.begin();

	int clockWiseCheckCount = 0;
	//return;
	while (roofPoints.size() > 3)
	{
		if (clockWiseCheckCount > roofPoints.size() * 2)
		{
			roofPoints.clear();
			roofTriangleEdges.clear();
			for (int q = points.size() - 2; q >= 0; q--)
			{
				RoofPoint roofPoint;
				roofPoint.id = q;
				roofPoint.point = points[q];
				roofPoint.point.z = _roof_level;

				roofPoints.push_back(roofPoint);
			}
			clockWiseCheckCount = 0;
			firstPoint = roofPoints.begin();
			roofPointsCopy = roofPoints;
		}

		secondPoint = nextLoop(firstPoint, roofPoints);
		thirdPoint = nextLoop(secondPoint, roofPoints);


		vector2D v1(firstPoint->point, secondPoint->point);
		vector2D v2(firstPoint->point, thirdPoint->point);

		if (!positiveAngle(v1, v2) && !anyPointInTriangle(firstPoint, secondPoint, thirdPoint))
		{
			//roofTriangleEdges.push_back({ *firstPoint, *thirdPoint});
			Triangle triangle;
			triangle.idp1 = firstPoint->id;
			triangle.idp2 = secondPoint->id;
			triangle.idp3 = thirdPoint->id;
			triangles.push_back(triangle);

			roofPoints.erase(secondPoint);
			clockWiseCheckCount = 0;
		}
		else
		{
			firstPoint = nextLoop(firstPoint, roofPoints);
			clockWiseCheckCount++;
		}
		//ce++;
		//if (ce > 27)
		//	break;
	}

	firstPoint = roofPoints.begin();
	secondPoint = nextLoop(firstPoint, roofPoints);
	thirdPoint = nextLoop(secondPoint, roofPoints);

	Triangle triangle;
	triangle.idp1 = firstPoint->id;
	triangle.idp2 = secondPoint->id;
	triangle.idp3 = thirdPoint->id;
	triangle.p1 = firstPoint;
	triangle.p2 = secondPoint;
	triangle.p3 = thirdPoint;
	triangles.push_back(triangle);

	roofPoints = roofPointsCopy;

	for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
	{
		for (auto itRoofPoint = roofPoints.begin(); itRoofPoint != roofPoints.end(); itRoofPoint++)
		{
			if (itTriangle->idp1 == itRoofPoint->id)
				itTriangle->p1 = itRoofPoint;
			if (itTriangle->idp2 == itRoofPoint->id)
				itTriangle->p2 = itRoofPoint;
			if (itTriangle->idp3 == itRoofPoint->id)
				itTriangle->p3 = itRoofPoint;
		}
	}

	std::list<std::list<RoofPoint>::iterator> firstWavefront;
	for (auto itRoofPoint = roofPoints.begin(); itRoofPoint != roofPoints.end(); itRoofPoint++)
	{
		firstWavefront.push_back(itRoofPoint);
	}
	wavefront.push_back(firstWavefront);

	for (auto itRoofPoint = roofPoints.begin(); itRoofPoint != roofPoints.end(); itRoofPoint++)
	{
		auto currentWavefront = wavefront.begin();
		auto pointOnWavefront = currentWavefront->begin();
		for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
		{
			for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
			{
				if ((*itWavefront)->id == itRoofPoint->id)
				{
					currentWavefront = itWavefrontList;
					pointOnWavefront = itWavefront;
					goto wavefrontFound;
				}
			}
		}
		wavefrontFound:

		auto nextPointOnWavefront = nextLoop(pointOnWavefront, *currentWavefront);
		auto previousPointOnWavefront = prevLoop(pointOnWavefront, *currentWavefront);

		calculateSpeedOfPoint(*pointOnWavefront, *nextPointOnWavefront, *previousPointOnWavefront);

	}

	for (int q = 0; q < 120; q++)
	{
		for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); )
		{
			if (itWavefrontList->size() == 3)
			{
				auto p1 = itWavefrontList->begin();
				auto p2 = nextLoop(p1, *itWavefrontList);
				auto p3 = nextLoop(p2, *itWavefrontList);
				removeTriangle(*p1, *p2, *p3);
				removeRoofPoint((*p1)->id);
				removeRoofPoint((*p2)->id);
				removeRoofPoint((*p3)->id);
				itWavefrontList = wavefront.erase(itWavefrontList);
				registerTriangles();
			}
			else
				itWavefrontList++;
		}

		bool egdeEvent = true;

		double minH = 100000.0f;
		auto edgeEventP1 = roofPoints.begin();
		auto edgeEventP2 = roofPoints.begin();

		auto flipSplitEventP1 = roofPoints.begin();
		auto flipSplitEventP2 = roofPoints.begin();
		auto flipSplitEventP3 = roofPoints.begin();

		for (auto itTriangle = triangles.begin(); itTriangle != triangles.end(); itTriangle++)
		{
			auto& p1 = itTriangle->p1;
			auto& p2 = itTriangle->p2;
			auto& p3 = itTriangle->p3;

			double joinTimeP1P2 = lineCollapseTime(p1, p2);
			if (joinTimeP1P2 < minH && joinTimeP1P2 >= 0.0f)
			{
				egdeEvent = true;
				minH = joinTimeP1P2;
				edgeEventP1 = p1;
				edgeEventP2 = p2;
			}

			double joinTimeP1P3 = lineCollapseTime(p1, p3);
			if (joinTimeP1P3 < minH && joinTimeP1P3 >= 0.0f)
			{
				egdeEvent = true;
				minH = joinTimeP1P3;
				edgeEventP1 = p1;
				edgeEventP2 = p3;
			}

			double joinTimeP2P3 = lineCollapseTime(p2, p3);
			if (joinTimeP2P3 < minH && joinTimeP2P3 >= 0.0f)
			{
				egdeEvent = true;
				minH = joinTimeP2P3;
				edgeEventP1 = p2;
				edgeEventP2 = p3;
			}

			double triangle123CollapseTime = triangleCollapseTime(p1, p2, p3);
			if (triangle123CollapseTime < minH && triangle123CollapseTime >= 0.0f &&
				futureDistance(p1, p2, triangle123CollapseTime) > 0.001f &&
				futureDistance(p2, p3, triangle123CollapseTime) > 0.001f &&
				futureDistance(p1, p3, triangle123CollapseTime) > 0.001f)
			{
				egdeEvent = false;
				minH = triangle123CollapseTime;
				flipSplitEventP1 = p1;
				flipSplitEventP2 = p2;
				flipSplitEventP3 = p3;
			}
		}
		auto yy = distanceIdId(13, 14);

		for (auto itRoofPoint = roofPoints.begin(); itRoofPoint != roofPoints.end(); itRoofPoint++)
		{
			Point next(itRoofPoint->point.x + minH * itRoofPoint->dx, itRoofPoint->point.y + minH * itRoofPoint->dy, itRoofPoint->point.z);
			roofLines.push_back({ itRoofPoint->point, next });

			itRoofPoint->point = next;
		}

		if (egdeEvent)
		{
			auto yyy = distanceIdId(23, 24);

			removeTriangle(edgeEventP1, edgeEventP2);

			//specPoints.push_back({ edgeEventP1->point, 0.0f, 1.0f, 0.0f });
			//specPoints.push_back({ edgeEventP2->point, 0.0f, 1.0f, 0.0f });

			removePointFromWavefront(edgeEventP2->id);
			renamePointInTriangles(edgeEventP2, edgeEventP1);
			roofPoints.erase(edgeEventP2);


			auto currentWavefront = wavefront.begin();
			auto pointOnWavefront = currentWavefront->begin();
			for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
			{
				for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
				{
					if ((*itWavefront)->id == edgeEventP1->id)
					{
						currentWavefront = itWavefrontList;
						pointOnWavefront = itWavefront;
						goto wavefrontFound2;
					}
				}
			}
			wavefrontFound2:

			auto nextPointOnWavefront = nextLoop(pointOnWavefront, *currentWavefront);
			auto previousPointOnWavefront = prevLoop(pointOnWavefront, *currentWavefront);

			calculateSpeedOfPoint(*pointOnWavefront, *nextPointOnWavefront, *previousPointOnWavefront);













			//newPoint.x += 

			//break;
		}
		else
		{
			//specPoints.push_back({ flipSplitEventP1->point, 1.0f, 0.0f, 0.0f });
			//specPoints.push_back({ flipSplitEventP2->point, 1.0f, 0.0f, 0.0f });
			//specPoints.push_back({ flipSplitEventP3->point, 1.0f, 0.0f, 0.0f });

			setCollisionPoint(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

			auto oppositePoint = oppositeTriangleWithEdge(flipSplitEventP2, flipSplitEventP3, flipSplitEventP1);

			if (oppositePoint != roofPoints.end()) //flip event
			{
				//specPoints.push_back({ oppositePoint->point, 0.0f, 0.0f, 1.0f });

				removeTriangle(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);
				removeTriangle(oppositePoint, flipSplitEventP2, flipSplitEventP3);

				Triangle t1;
				t1.p1 = flipSplitEventP1;
				t1.p2 = flipSplitEventP2;
				t1.p3 = oppositePoint;
				t1.idp1 = flipSplitEventP1->id;
				t1.idp2 = flipSplitEventP2->id;
				t1.idp3 = oppositePoint->id;

				Triangle t2;
				t2.p1 = flipSplitEventP1;
				t2.p2 = flipSplitEventP3;
				t2.p3 = oppositePoint;
				t2.idp1 = flipSplitEventP1->id;
				t2.idp2 = flipSplitEventP3->id;
				t2.idp3 = oppositePoint->id;

				triangles.push_back(t1);
				triangles.push_back(t2);


			}
			else //split event
			{

				int gg = 5;

				setCollisionPoint(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

				removeTriangle(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);
				flipSplitEventP1->dx = 0;
				flipSplitEventP1->dy = 0;
				RoofPoint newPoint = *flipSplitEventP1;
				long long newId = nextRoofPointid();
				long long oldId = flipSplitEventP1->id;
				newPoint.id = newId;
				roofPoints.push_back(newPoint);


				auto currentWavefront = wavefront.begin();
				auto pointOnWavefront = currentWavefront->begin();
				for (auto itWavefrontList = wavefront.begin(); itWavefrontList != wavefront.end(); itWavefrontList++)
				{
					for (auto itWavefront = (*itWavefrontList).begin(); itWavefront != (*itWavefrontList).end(); itWavefront++)
					{
						if ((*itWavefront)->id == flipSplitEventP1->id)
						{
							currentWavefront = itWavefrontList;
							pointOnWavefront = itWavefront;
							goto wavefrontFound3;
						}
					}
				}
				wavefrontFound3:

				std::list<std::list<RoofPoint>::iterator> l1;
				std::list<std::list<RoofPoint>::iterator> l2;

				auto nextInWavefront = pointOnWavefront;

				for (auto it = pointOnWavefront; (*it)->id != flipSplitEventP3->id; it = nextLoop(it, (*currentWavefront)))
				{
					l1.push_back(*it);
					nextInWavefront = it;
				}
				nextInWavefront = nextLoop(nextInWavefront, (*currentWavefront));

				l2.push_back(std::prev(roofPoints.end()));
				for (auto it = nextInWavefront; it != pointOnWavefront; it = nextLoop(it, (*currentWavefront)))
				{
					l2.push_back(*it);
				}

				for (auto itl1 = l1.begin(); itl1 != l1.end(); itl1++)
				{
					//specPoints.push_back({ (*itl1)->point, 0.0f, 1.0f, 1.0f });
				}

				for (auto itl2 = l2.begin(); itl2 != l2.end(); itl2++)
				{
					specPoints.push_back({ (*itl2)->point, 1.0f, 1.0f, 0.0f });
				}

				wavefront.erase(currentWavefront);
				wavefront.push_back(l1);
				wavefront.push_back(l2);


				calculateSpeedOfPoint(newPoint.id);
				calculateSpeedOfPoint(flipSplitEventP1->id);

				//verticiesList.push_back(l1);
				//verticiesList.push_back(l2);
				//verticiesList.erase(currentVerticiesLoop);

				for(auto it = l2.begin(); it != l2.end(); it++)
				{
					for (auto itTrian = triangles.begin(); itTrian != triangles.end(); itTrian++)
					{
						if (itTrian->idp1 == (*it)->id || itTrian->idp2 == (*it)->id || itTrian->idp3 == (*it)->id )
						{
							if (itTrian->idp1 == oldId)
							{
								itTrian->idp1 = newId;
								itTrian->p1 = std::prev(roofPoints.end());
							}
							if (itTrian->idp2 == oldId)
							{
								itTrian->idp2 = newId;
								itTrian->p2 = std::prev(roofPoints.end());
							}
							if (itTrian->idp3 == oldId)
							{
								itTrian->idp3 = newId;
								itTrian->p3 = std::prev(roofPoints.end());
							}
						}

					}
				}

				registerTriangles();

				break;

			}


		}
		


		auto g = distanceIdId(24, 23);
		auto x = 5;
		registerTriangles();
	}



	auto id = getId();












	//return;
	//skip = true;
	/*if (id == 101192162 || id == 101202189 || id == 101192162 || id == 101209638 || id == 101209921 || id == 101215614)
	{
	if (id == 101215614)
	skip = true;
	}
	else */
	return;









	for (size_t q = 0; q < points.size() - 1; q++)
	{
		Point& currentPoint = points[q];

		Point& nextPoint = points[q + 1];
		Point& previousPoint = points[(q - 1) % (points.size())];

		double alpha = measureAngle(currentPoint, previousPoint, nextPoint) / 2;
		double beta = atan(tan(gamma) * sin(alpha));

		Line2D firstLine(currentPoint, nextPoint);
		Line2D secondLine(currentPoint, previousPoint);

		Line2D firstBisector;
		Line2D secondBisector;

		firstBisector.calculateBisector(firstLine, secondLine);
		secondBisector.calculateBisector(firstLine, secondLine, true);


		double r = 0.05f;

		std::vector<Point> pointsToCheck;

		//get any point on bisector

		for (auto& line : { firstBisector , secondBisector })
		{
			float x;
			float y;
			if (firstBisector.B != 0)
			{
				x = 1.0f;
				y = (-line.A - line.C) / line.B;
			}
			else
			{
				x = -line.C / line.A;
				y = 1;
			}

			Point vec{ x - currentPoint.x, y - currentPoint.y };
			float distance = vec.distance2D(Point{ 0.0f, 0.0f });

			x = currentPoint.x + vec.x * r / distance;
			y = currentPoint.y + vec.y * r / distance;

			pointsToCheck.push_back({ x, y, _roof_level });
			//pointsToCheck.push_back({ x, y, _roof_level + 1.0f });

			x = currentPoint.x + vec.x * (-r) / distance;
			y = currentPoint.y + vec.y * (-r) / distance;

			pointsToCheck.push_back({ x, y, _roof_level });
			//pointsToCheck.push_back({ x, y, _roof_level + 1.0f });
		}

		std::swap(pointsToCheck[1], pointsToCheck[2]);


		PointInsidePolygonDetector pointInsidePolygonDetector;

		bool pInside[4];
		pInside[0] = pointInsidePolygonDetector.isInside(points, pointsToCheck[0]);
		pInside[1] = pointInsidePolygonDetector.isInside(points, pointsToCheck[1]);
		pInside[2] = pointInsidePolygonDetector.isInside(points, pointsToCheck[2]);
		pInside[3] = pointInsidePolygonDetector.isInside(points, pointsToCheck[3]);

		for (int w = 0; w < 4; w++)
		{
			if (pInside[w] == true && pInside[(w + 2) % 4] == false)
			{
				specialPoints.push_back({ pointsToCheck[w].x, pointsToCheck[w].y, _roof_level });
				specialPoints.push_back({ pointsToCheck[w].x, pointsToCheck[w].y, _roof_level + 1 });

				RoofData newElement;

				currentPoint.z = _roof_level;

				newElement.startPoint = currentPoint;
				newElement.firstPoint = currentPoint;
				newElement.firstPointIsNull = false;
				newElement.directionPoint = pointsToCheck[w];
				newElement.beta = beta;

				roofData.push_back(newElement);


				vertex newVertex;
				newVertex.startPoint = currentPoint;
				newVertex.edgePointStart = currentPoint;
				Point vec(pointsToCheck[w].x - currentPoint.x, pointsToCheck[w].y - currentPoint.y);
				float dist = vec.distance2D(Point( 0.0f, 0.0f ));
				vec.x *= 1.0f / dist;
				vec.y *= 1.0f / dist;

				newVertex.dv = 1.0f / tan(beta);
				newVertex.dx = vec.x / tan(beta);
				newVertex.dy = vec.y / tan(beta);
				newVertex.beta = beta;

				vertices.push_back(newVertex);

				break;
			}
		}
	}


	{
		auto it = vertices.begin();
		while (it != vertices.end())
		{
			if (std::next(it) != vertices.end())
			{
				auto it2 = std::next(it);
				if (std::next(it2) != vertices.end())
				{
					auto it3 = std::next(it2);

					Point p1 = it->startPoint;
					Point p2 = it2->startPoint;
					Point p3 = it3->startPoint;

					Line2D l1(p1, p2);
					Line2D l2(p2, p3);
					Line2D l(p1, p3);


					if (l.pointDistance(p2) < 0.05 || l1.isSame(l2))
					{
						vertices.erase(it2);
						it = vertices.begin();
					}
				}
			}



			it++;
		}
	}





	auto currentVertex = vertices.begin();
	auto nextVertex = vertices.begin();





	auto firstSmallest = vertices.begin();
	auto secondSmallest = vertices.begin();

	auto waveFrontStartSmallest = vertices.begin();
	auto waveFrontEndSmallest = vertices.begin();
	auto currentVerticiesLoop = verticiesList.begin();

	Point intersectionSmallest;

	float lastMinHeight = _roof_level;

	verticiesList.push_back(vertices);

	int c = 0;
	float lastH = 0.0f;

	while (verticiesList.size() > 0)
	{
	startLoop:

		for (auto listIt = verticiesList.begin(); listIt != verticiesList.end(); )
		{
			if ((*listIt).size() < 3)
			{
				ridges.push_back({ (*listIt).begin()->edgePointStart, nextLoop((*listIt).begin(), *listIt)->edgePointStart });
				listIt = verticiesList.erase(listIt);
				goto startLoop;
			}
			else
				listIt++;

		}

		if (verticiesList.empty())
			break;


		float minHeightEdgeEvent = 100000.0f;
		float minHeightSplitEvent = 100000.0f;



		//EDGE EVENT
		for (auto listIt = verticiesList.begin(); listIt != verticiesList.end(); listIt++)
		{
			if (0)
			{
				for (auto i = (*listIt).begin(); i != (*listIt).end(); i++)
					specPoints.push_back({i->startPoint, 1.0f, 0.5f, 0.1f});
			}
			int ccc = 0;
			for (auto i = (*listIt).begin(); i != (*listIt).end(); i++)
			{
				ccc++;

				if (ccc == 8 && c == 14)
				{
					auto h = 5;
				}

				currentVertex = i;
				nextVertex = (std::next(i) != (*listIt).end()) ? std::next(i) : (*listIt).begin();

				Line2D l1(currentVertex->startPoint, Point(currentVertex->startPoint.x + currentVertex->dx, currentVertex->startPoint.y + currentVertex->dy));
				Line2D l2(nextVertex->startPoint, Point(nextVertex->startPoint.x + nextVertex->dx, nextVertex->startPoint.y + nextVertex->dy));

				Point intersectionPoint = l1.calcuateIntersectionPoint(l2);

				float height;

				bool direction = (currentVertex->dx) * (intersectionPoint.x - currentVertex->startPoint.x) > 0 ? true : false;

				if (currentVertex->startPoint.distance2D(intersectionPoint) < nextVertex->startPoint.distance2D(intersectionPoint))
					height = currentVertex->startPoint.distance2D(intersectionPoint) * tan(currentVertex->beta) * (direction * 2 - 1);
				else
					height = nextVertex->startPoint.distance2D(intersectionPoint) * tan(nextVertex->beta) * (direction * 2 - 1);

				intersectionPoint.z = height + _roof_level;



				if (height < minHeightEdgeEvent && height > 0)
				{
					minHeightEdgeEvent = height;

					firstSmallest = currentVertex;
					secondSmallest = nextVertex;
					intersectionSmallest = intersectionPoint;
					currentVerticiesLoop = listIt;
				}
			}
		}

		//SPLIT EVENT
		float SE_minH = 100000.f;
		int hhh = 0;
		for (auto listIt = verticiesList.begin(); listIt != verticiesList.end(); listIt++)
		{
			for (auto i = (*listIt).begin(); i != (*listIt).end(); i++)
			{
				currentVertex = i;
				auto wavefrontStart = nextLoop(i, (*listIt));
				auto wavefrontEnd = nextLoop(i, (*listIt), 2);
				hhh++;
				if (hhh == 10 && c == 14)
				{
					specPoints.push_back({ (*currentVertex).startPoint, 0.0f, 0.8f, 0.2f});
				}

				while (wavefrontEnd != currentVertex)
				{
					float a = wavefrontStart->startPoint.x - wavefrontEnd->startPoint.x;
					float b = wavefrontStart->dx - wavefrontEnd->dx;
					float c = wavefrontStart->startPoint.y - wavefrontEnd->startPoint.y;
					float d = wavefrontStart->dy - wavefrontEnd->dy;
					float e = wavefrontStart->startPoint.x - currentVertex->startPoint.x;
					float f = wavefrontStart->dx - currentVertex->dx;
					float g = wavefrontStart->startPoint.y - currentVertex->startPoint.y;
					float h = wavefrontStart->dy - currentVertex->dy;

					float delta = sqrt(pow((-a*h - b*g + c*f + d*e), 2) - 4 * (c*e - a*g)*(d*f - b*h));

					float h1 = (delta + a*h + b*g - c*f - d*e) / (2 * (d*f - b*h));
					float h2 = (-delta + a*h + b*g - c*f - d*e) / (2 * (d*f - b*h));

					if (h1 < SE_minH && h1 > lastH || h2 < SE_minH && h2 > lastH)
					{
						if (h1 < SE_minH && h1 > lastH)
							SE_minH = h1;
						else
							SE_minH = h2;
					}


					if (h1 < minHeightEdgeEvent && h1 > 0 || h2 < minHeightEdgeEvent && h2 > 0)
					{
						float correctH;
						if (h1 < minHeightEdgeEvent && h1 > 0)
							correctH = h1;
						else
							correctH = h2;

						Point futureCurrentPoint = currentVertex->startPoint;
						futureCurrentPoint.x += currentVertex->dx * correctH;
						futureCurrentPoint.y += currentVertex->dy * correctH;
						futureCurrentPoint.z = correctH + _roof_level;

						Point futureStartPoint = wavefrontStart->startPoint;
						futureStartPoint.x += wavefrontStart->dx * correctH;
						futureStartPoint.y += wavefrontStart->dy * correctH;

						Point futureEndPoint = wavefrontEnd->startPoint;
						futureEndPoint.x += wavefrontEnd->dx * correctH;
						futureEndPoint.y += wavefrontEnd->dy * correctH;

						float maxX = std::max(futureStartPoint.x, futureEndPoint.x);
						float maxY = std::max(futureStartPoint.y, futureEndPoint.y);
						float minX = std::min(futureStartPoint.x, futureEndPoint.x);
						float minY = std::min(futureStartPoint.y, futureEndPoint.y);

						if (futureCurrentPoint.x < maxX && futureCurrentPoint.x > minX && futureCurrentPoint.y < maxY && futureCurrentPoint.y > minY)
						{
							auto kkk = 5;
							specPoints.push_back({ futureCurrentPoint, 0.0f, 0.0f, 1.0f });
							minHeightSplitEvent = correctH;
							firstSmallest = currentVertex;
							waveFrontStartSmallest = wavefrontStart;
							waveFrontEndSmallest = wavefrontEnd;
							currentVerticiesLoop = listIt;
							intersectionSmallest = futureCurrentPoint;
						}

					}

					wavefrontStart = nextLoop(wavefrontStart, (*listIt));
					wavefrontEnd = nextLoop(wavefrontEnd, (*listIt));
				}

			}
		}

		for (auto listIt = verticiesList.begin(); listIt != verticiesList.end(); listIt++)
		{
			for (auto i = (*listIt).begin(); i != (*listIt).end(); i++)
			{
				auto nextVertex = (std::next(i) != (*listIt).end()) ? std::next(i) : (*listIt).begin();
				Point p1 = i->startPoint;
				p1.x += i->dx * std::min(minHeightEdgeEvent, minHeightSplitEvent);
				p1.y += i->dy * std::min(minHeightEdgeEvent, minHeightSplitEvent);
				p1.z = _roof_level + std::min(minHeightEdgeEvent, minHeightSplitEvent);
				Point p2 = nextVertex->startPoint;
				p2.x += nextVertex->dx * std::min(minHeightEdgeEvent, minHeightSplitEvent);
				p2.y += nextVertex->dy * std::min(minHeightEdgeEvent, minHeightSplitEvent);
				p2.z = _roof_level + std::min(minHeightEdgeEvent, minHeightSplitEvent);

				//izolines.push_back({ p1, p2 });
				lastH = std::min(minHeightEdgeEvent, minHeightSplitEvent);
			}
		}
		


		//EVENT HANDLING
		if (minHeightSplitEvent < minHeightEdgeEvent) //split event
		{
			ridges.push_back({ firstSmallest->edgePointStart, intersectionSmallest });

			std::list<vertex> l1;
			std::list<vertex> l2;

			//firstSmallest'
			{
				auto nextFirstSmallest = nextLoop(firstSmallest, *currentVerticiesLoop);

				Line2D firstEaves(firstSmallest->startPoint, nextFirstSmallest->startPoint);
				Line2D secondEaves(waveFrontStartSmallest->startPoint, waveFrontEndSmallest->startPoint);

				Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
				newIntersection.z = _roof_level;

				float alpha = measureAngle(newIntersection, nextFirstSmallest->startPoint, waveFrontEndSmallest->startPoint) / 2;
				float beta = atan(tan(gamma) * sin(alpha));


				vertex newVertex;
				newVertex.startPoint = newIntersection;
				newVertex.edgePointStart = intersectionSmallest;
				Point vec(intersectionSmallest.x - newIntersection.x, intersectionSmallest.y - newIntersection.y);
				float dist = vec.distance2D(Point(0.0f, 0.0f));
				vec.x *= 1.0f / dist;
				vec.y *= 1.0f / dist;

				newVertex.dv = 1.0f / tan(beta);
				newVertex.dx = vec.x / tan(beta);
				newVertex.dy = vec.y / tan(beta);
				newVertex.beta = beta;

				l1.push_back(newVertex);
				specPoints.push_back({ newVertex.startPoint, 1.0f, 0.0f, 0.0f });
			}

			//firstSmallest''
			{
				auto previousFirstSmallest = (firstSmallest != (*currentVerticiesLoop).begin()) ? std::prev(firstSmallest) : prev((*currentVerticiesLoop).end());
				
				Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
				Line2D secondEaves(waveFrontStartSmallest->startPoint, waveFrontEndSmallest->startPoint);

				Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
				newIntersection.z = _roof_level;

				float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, waveFrontEndSmallest->startPoint) / 2;
				float beta = atan(tan(gamma) * sin(alpha));


				vertex newVertex;
				newVertex.startPoint = newIntersection;
				newVertex.edgePointStart = intersectionSmallest;
				Point vec(intersectionSmallest.x - newIntersection.x, intersectionSmallest.y - newIntersection.y);
				float dist = vec.distance2D(Point(0.0f, 0.0f));
				vec.x *= 1.0f / dist;
				vec.y *= 1.0f / dist;

				newVertex.dv = 1.0f / tan(beta);
				newVertex.dx = vec.x / tan(beta);
				newVertex.dy = vec.y / tan(beta);
				newVertex.beta = beta;

				l2.push_back(newVertex);
				specPoints.push_back({ newVertex.startPoint, 1.0f, 0.0f, 0.0f });
			}

			for (auto it = nextLoop(firstSmallest, *currentVerticiesLoop); it != waveFrontEndSmallest; it = nextLoop(it, (*currentVerticiesLoop)))
			{
				l1.push_back(*it);
			}

			for (auto it = waveFrontEndSmallest; it != firstSmallest; it = nextLoop(it, (*currentVerticiesLoop)))
			{
				l2.push_back(*it);
			}

			for (auto itl1 = l1.begin(); itl1 != l1.end(); itl1++)
			{
				specPoints.push_back({ itl1->startPoint, 0.0f, 1.0f, 1.0f });
			}

			for (auto itl2 = l2.begin(); itl2 != l2.end(); itl2++)
			{
				specPoints.push_back({ itl2->startPoint, 1.0f, 1.0f, 0.0f });
			}

			verticiesList.push_back(l1);
			verticiesList.push_back(l2);
			verticiesList.erase(currentVerticiesLoop);
		}

		else //edge event
		{

			ridges.push_back({ firstSmallest->edgePointStart, intersectionSmallest });
			ridges.push_back({ secondSmallest->edgePointStart, intersectionSmallest });


			auto previousFirstSmallest = (firstSmallest != (*currentVerticiesLoop).begin()) ? std::prev(firstSmallest) : prev((*currentVerticiesLoop).end());
			auto nextSecondSmallest = (std::next(secondSmallest) != (*currentVerticiesLoop).end()) ? std::next(secondSmallest) : (*currentVerticiesLoop).begin();

			Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
			Line2D secondEaves(secondSmallest->startPoint, nextSecondSmallest->startPoint);

			Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
			newIntersection.z = _roof_level;

			float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, nextSecondSmallest->startPoint) / 2;
			float beta = atan(tan(gamma) * sin(alpha));



			vertex newVertex;
			newVertex.startPoint = newIntersection;
			newVertex.edgePointStart = intersectionSmallest;
			Point vec(intersectionSmallest.x - newIntersection.x, intersectionSmallest.y - newIntersection.y);
			float dist = vec.distance2D(Point(0.0f, 0.0f));
			vec.x *= 1.0f / dist;
			vec.y *= 1.0f / dist;

			newVertex.dv = 1.0f / tan(beta);
			newVertex.dx = vec.x / tan(beta);
			newVertex.dy = vec.y / tan(beta);
			newVertex.beta = beta;


			*firstSmallest = newVertex;
			(*currentVerticiesLoop).erase(secondSmallest);
		}
		
		c++;

		if (c > 10)
			break;
	}


	int h = 8;



	
	



	/*float maxX = -1000000.0;
	float minX = 1000000.0;
	float maxY = -1000000.0;
	float minY = 1000000.0;


	for (auto it = roofData.begin(); it != roofData.end(); it++)
	{
		if (it->startPoint.x > maxX)
			maxX = it->startPoint.x;
		if (it->startPoint.y > maxY)
			maxY = it->startPoint.y;

		if (it->startPoint.x < minX)
			minX = it->startPoint.x;
		if (it->startPoint.y < minY)
			minY = it->startPoint.y;
	}



	PointInsidePolygonDetector pointInsidePolygonDetector;

	auto currentPoint = roofData.begin();
	auto nextPoint = roofData.begin();

	std::vector<Plane> planes;
	std::list<Point> roofPointsToBeProcessed;

	for (auto i = roofData.begin(); i != roofData.end(); ++i)
	{
		currentPoint = i;
		nextPoint = (std::next(i) != roofData.end()) ? std::next(i) : roofData.begin();




		float x = (currentPoint->startPoint.x + nextPoint->startPoint.x) / 2;
		float y = (currentPoint->startPoint.y + nextPoint->startPoint.y) / 2;

		Point newPoint(x, y, _roof_level);
		specPoints.push_back({ newPoint , 0.9f, 0.2f, 0.2f });

		Point vec{ x - currentPoint->startPoint.x, y - currentPoint->startPoint.y };

		float delta = 0.05; //[m]
		float scaleRatio = delta / newPoint.distance2D(currentPoint->startPoint);
		vec.x *= scaleRatio;
		vec.y *= scaleRatio;

		Point vecRotated{-vec.y, vec.x };

		Point translated1{ newPoint.x + vecRotated.x, newPoint.y + vecRotated.y, _roof_level };
		Point translated2{ newPoint.x - vecRotated.x, newPoint.y - vecRotated.y, _roof_level };

		Point finalPoint;

		if (pointInsidePolygonDetector.isInside(points, translated1))
		{
			finalPoint = translated1;
		}
		else
		{
			finalPoint = translated2;
		}

		finalPoint.z = tan(gamma) * delta + _roof_level;
		//specPoints.push_back({ finalPoint , 0.9f, 1.0f, 0.2f });

		Plane Pl(currentPoint->startPoint, nextPoint->startPoint, finalPoint);
		planes.push_back(std::move(Pl));


	}




	for (auto y = minY; y <= maxY; y += 0.5f)
	{
		for (auto x = minX; x <= maxX; x += 0.5f)
		{
			Point newPoint(x, y, _roof_level);

			if (pointInsidePolygonDetector.isInside(points, newPoint))
				//specPoints.push_back({ newPoint , 0.1f, 0.5f, 0.9f});
			{
				roofPointsToBeProcessed.push_back(newPoint);
			}
		}
	}

	for (float d = _roof_level; d < 50.0f; d += 0.1f)
	{
		for (auto it = roofPointsToBeProcessed.begin(); it != roofPointsToBeProcessed.end(); )
		{
			int roofLevelLimitPlanes = 0;
			int deltaLevelLimitPlanes = 0;
			for (auto& plane : planes)
			{
				if (d < (-it->x * plane.A - it->y * plane.B) / plane.C)
				{
					deltaLevelLimitPlanes++;
				}
				if (_roof_level < (-it->x * plane.A - it->y * plane.B) / plane.C)
				{
					roofLevelLimitPlanes++;
				}
			}
			if (roofLevelLimitPlanes > deltaLevelLimitPlanes)
			{
				it->z = d;
				specPoints.push_back({ *it , 0.1f, 0.5f, 0.9f });
				it = roofPointsToBeProcessed.erase(it);
			}
			else
			{
				it++;
			}
		}
	}*/


	/*
	float H = _roof_level;
	float h = 0.0f;

	std::list<std::list<RoofData>::iterator> waitingPoint;

	{
		auto it = roofData.begin();
		while (it != roofData.end())
		{
			if (std::next(it) != roofData.end())
			{
				auto it2 = std::next(it);
				if (std::next(it2) != roofData.end())
				{
					auto it3 = std::next(it2);

					Point p1 = it->startPoint;
					Point p2 = it2->startPoint;
					Point p3 = it3->startPoint;

					Line2D l1(p1, p2);
					Line2D l2(p2, p3);
					Line2D l(p1, p3);


					if (l.pointDistance(p2) < 0.05 || l1.isSame(l2))
					{
						roofData.erase(it2);
						it = roofData.begin();
					}
				}
			}



			it++;
		}
	}

	//for (auto it = roofData.begin(); it != roofData.end(); it++)
	//{
	//	specPoints.push_back({ it->startPoint, 0.1f, 0.1f, 0.7f });
	//}
	//


	while (roofData.size() > 5)
	{

		float smallestHeight = 100000;

		auto currentPoint = roofData.begin();
		auto nextPoint = roofData.begin();
		auto previousPoint = roofData.begin();

		auto firstSmallest = roofData.begin();
		auto secondSmallest = roofData.begin();
		Point intersectionPointSmallest;

		int c = 0;

		for (auto i = roofData.begin(); i != roofData.end(); ++i)
		{
			c++;

			currentPoint = i;
			nextPoint = (std::next(i) != roofData.end()) ? std::next(i) : roofData.begin();

			Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
			Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

			Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

			bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

			float distance2Dfirst = intersectionPoint.distance2D(currentPoint->startPoint);
			float distance2Dsecond = intersectionPoint.distance2D(nextPoint->startPoint);

			if (distance2Dfirst < distance2Dsecond)
				intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);
			else
				intersectionPoint.z = H + (tan(nextPoint->beta) * intersectionPoint.distance2D(nextPoint->startPoint)) * (direction * 2 - 1);

			if (intersectionPoint.z > H && intersectionPoint.z < smallestHeight)
			{

				bool currentPointBisectionIsRising = currentPoint->startPoint.distance2D(intersectionPoint) > currentPoint->startPoint.distance2D(currentPoint->directionPoint);
				bool nextPointBisectionIsRising = nextPoint->startPoint.distance2D(intersectionPoint) > nextPoint->startPoint.distance2D(nextPoint->directionPoint);

				if (currentPointBisectionIsRising && nextPointBisectionIsRising)
				{
					firstSmallest = currentPoint;
					secondSmallest = nextPoint;
					intersectionPointSmallest = intersectionPoint;
					smallestHeight = intersectionPoint.z;
				}
			}
		}

		float currentPointHeight = smallestHeight;

		intersectionPointSmallest.z = smallestHeight;
					
		auto previousFirstSmallest = (firstSmallest != roofData.begin()) ? std::prev(firstSmallest) : prev(roofData.end());
		auto nextSecondSmallest = (std::next(secondSmallest) != roofData.end()) ? std::next(secondSmallest) : roofData.begin();

		Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
		Line2D secondEaves(secondSmallest->startPoint, nextSecondSmallest->startPoint);

		Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
		newIntersection.z = _roof_level;

		float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, nextSecondSmallest->startPoint) / 2;
		float beta = atan(tan(gamma) * sin(alpha));

		if (roofData.size() == 7)
		{
			specPoints.push_back({ firstSmallest->firstPoint , 0.5f, 0.1f, 0.7f});
			specPoints.push_back({ secondSmallest->firstPoint , 0.5f, 0.1f, 0.7f});
		}


		roofEdges.push_back({ firstSmallest->firstPoint, intersectionPointSmallest });
		roofEdges.push_back({ secondSmallest->firstPoint, intersectionPointSmallest });

		*firstSmallest = RoofData{ newIntersection , intersectionPointSmallest, true, intersectionPointSmallest, intersectionPointSmallest, beta };
		roofData.erase(secondSmallest);
	

		if (roofData.size() == 6)
		{
			int h = 5;
			break;
		}			



		float max = -1.0f;


		for (auto i = roofData.begin(); i != roofData.end(); ++i)
		{
			c++;

			currentPoint = i;
			nextPoint = (std::next(i) != roofData.end()) ? std::next(i) : roofData.begin();

			Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
			Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

			Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

			bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

			float distance2Dfirst = intersectionPoint.distance2D(currentPoint->startPoint);
			float distance2Dsecond = intersectionPoint.distance2D(nextPoint->startPoint);

			if (distance2Dfirst < distance2Dsecond)
				intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);
			else
				intersectionPoint.z = H + (tan(nextPoint->beta) * intersectionPoint.distance2D(nextPoint->startPoint)) * (direction * 2 - 1);

			if (intersectionPoint.z < currentPointHeight && intersectionPoint.z > max && intersectionPoint.z > _roof_level)
			{
				firstSmallest = currentPoint;
				secondSmallest = nextPoint;
				intersectionPointSmallest = intersectionPoint;
				max = intersectionPoint.z;
			}
		}

		if (max > 0.0f)
		{
			currentPointHeight = max;

			intersectionPointSmallest.z = max;

			auto previousFirstSmallest = (firstSmallest != roofData.begin()) ? std::prev(firstSmallest) : prev(roofData.end());
			auto nextSecondSmallest = (std::next(secondSmallest) != roofData.end()) ? std::next(secondSmallest) : roofData.begin();

			Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
			Line2D secondEaves(secondSmallest->startPoint, nextSecondSmallest->startPoint);

			Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
			newIntersection.z = _roof_level;

			float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, nextSecondSmallest->startPoint) / 2;
			float beta = atan(tan(gamma) * sin(alpha));


			roofEdges.push_back({ firstSmallest->firstPoint, intersectionPointSmallest });
			roofEdges.push_back({ secondSmallest->firstPoint, intersectionPointSmallest });

			*firstSmallest = RoofData{ newIntersection , intersectionPointSmallest, true, intersectionPointSmallest, intersectionPointSmallest, beta };
			roofData.erase(secondSmallest);

		}










	}










	if (roofData.size() == 2)
	{
		roofEdges.push_back({ roofData.front().firstPoint, roofData.back().firstPoint });

	}

	if (waitingPoint.size())
		specPoints.push_back({ waitingPoint.front()->startPoint, 0.1f, 1.0f, 0.2f });

	for (auto it = roofData.begin(); it != roofData.end(); it++)
	{
		//specPoints.push_back({ it->startPoint, 0.8f, 0.1f, 0.7f });
		//specPoints.push_back({ it->directionPoint, 0.2f, 0.1f, 0.7f });
	}


















	/*

















	for (int q = 0; q < 1; q++)
	{
	float smallestHeight = 100000;

	auto currentPoint = roofData.begin();
	auto nextPoint = roofData.begin();
	auto previousPoint = roofData.begin();

	auto firstSmallest = roofData.begin();
	auto secondSmallest = roofData.begin();
	Point intersectionPointSmallest;

	int c = 0;

	for (auto i = roofData.begin(); i != roofData.end(); ++i)
	{
	c++;

	currentPoint = i;
	nextPoint = (std::next(i) != roofData.end()) ? std::next(i) : roofData.begin();

	Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
	Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

	Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

	bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

	intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);

	if (intersectionPoint.z > H && intersectionPoint.z < smallestHeight
	//&& currentPoint->startPoint.distance2D(intersectionPoint) > currentPoint->startPoint.distance2D(currentPoint->directionPoint)
	//&& nextPoint->startPoint.distance2D(intersectionPoint) > nextPoint->startPoint.distance2D(nextPoint->directionPoint)
	)
	{
	firstSmallest = currentPoint;
	secondSmallest = nextPoint;
	intersectionPointSmallest = intersectionPoint;
	smallestHeight = intersectionPoint.z;
	}
	int h = 5;




	}

	if (q == 3)
	int gg = 5;


	intersectionPointSmallest.z = smallestHeight;

	auto previousFirstSmallest = (firstSmallest != roofData.begin()) ? std::prev(firstSmallest) : prev(roofData.end());
	auto nextSecondSmallest = (std::next(secondSmallest) != roofData.end()) ? std::next(secondSmallest) : roofData.begin();

	Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
	Line2D secondEaves(secondSmallest->startPoint, nextSecondSmallest->startPoint);

	Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
	newIntersection.z = _roof_level;

	float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, nextSecondSmallest->startPoint) / 2;
	float beta = atan(tan(gamma) * sin(alpha));


	roofEdges.push_back({ firstSmallest->firstPoint, intersectionPointSmallest });
	roofEdges.push_back({ secondSmallest->firstPoint, intersectionPointSmallest });

	*firstSmallest = RoofData{ newIntersection , intersectionPointSmallest, true, intersectionPointSmallest, intersectionPointSmallest, beta };
	roofData.erase(secondSmallest);

	}





































	for (int q = 0; q < 1; q++)
	{
	float smallestHeight = 100000;

	auto currentPoint = roofData.begin();
	auto nextPoint = roofData.begin();
	auto previousPoint = roofData.begin();

	auto firstSmallest = roofData.begin();
	auto secondSmallest = roofData.begin();
	Point intersectionPointSmallest;

	int c = 0;


	float realDistance = 0;




	for (auto i = roofData.begin(); i != roofData.end(); ++i)
	{
	c++;


	currentPoint = i;
	nextPoint = (std::next(i) != roofData.end()) ? std::next(i) : roofData.begin();

	Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
	Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

	Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

	if (c == 3 && q == 0)
	{
	int yyyy = 0;


	//firstSmallest = currentPoint;
	//secondSmallest = nextPoint;
	//intersectionPointSmallest = nextPoint;
	}

	bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

	intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);

	if (intersectionPoint.z > H && intersectionPoint.z < smallestHeight
	//&& currentPoint->startPoint.distance2D(intersectionPoint) > currentPoint->startPoint.distance2D(currentPoint->directionPoint)
	//&& nextPoint->startPoint.distance2D(intersectionPoint) > nextPoint->startPoint.distance2D(nextPoint->directionPoint)
	)
	{
	firstSmallest = currentPoint;
	secondSmallest = nextPoint;
	intersectionPointSmallest = intersectionPoint;
	smallestHeight = intersectionPoint.z;

	}
	int h = 5;




	}

	specPoints.push_back(std::make_tuple(secondSmallest->directionPoint, 1.0f, 0.0f, 1.0f));
	specPoints.push_back(std::make_tuple(secondSmallest->startPoint, 1.0f, 1.0f, 1.0f));

	specPoints.push_back(std::make_tuple(firstSmallest->directionPoint, 0.0f, 0.0f, 1.0f));
	specPoints.push_back(std::make_tuple(firstSmallest->startPoint, 0.0f, 1.0f, 1.0f));
	//specPoints.push_back(std::make_tuple(Point( 30,30,50 ), 1.0f, 1.0f, 0.0f));

	specialPoints.push_back(secondSmallest->directionPoint);
	auto xxx = secondSmallest->directionPoint;
	xxx.z = 100;
	specialPoints.push_back(xxx);


	realDistance = secondSmallest->directionPoint.distance2D(intersectionPointSmallest);

	previousPoint = (firstSmallest != roofData.begin()) ? std::prev(firstSmallest) : prev(roofData.end());

	specPoints.push_back(std::make_tuple(previousPoint->startPoint, 0.5f, 0.5f, 1.0f));
	specPoints.push_back(std::make_tuple(previousPoint->firstPoint, 0.5f, 0.5f, 0.0f));

	specPoints.push_back(std::make_tuple(nextPoint->startPoint, 0.5f, 0.5f, 1.0f));
	specPoints.push_back(std::make_tuple(nextPoint->firstPoint, 0.5f, 0.5f, 0.0f));

	Line2D firstBisection(secondSmallest->startPoint, secondSmallest->directionPoint);
	Line2D secondBisection(previousPoint->startPoint, previousPoint->directionPoint);

	Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

	bool direction = (secondSmallest->directionPoint.x - secondSmallest->startPoint.x) * (intersectionPoint.x - secondSmallest->startPoint.x) > 0 ? true : false;

	intersectionPoint.z = H + (tan(secondSmallest->beta) * intersectionPoint.distance2D(secondSmallest->startPoint)) * (direction * 2 - 1);


	float realDistance2 = firstSmallest->directionPoint.distance2D(intersectionPoint);


	if (//intersectionPoint.z > H && intersectionPoint.z < smallestHeight
	//&& currentPoint->startPoint.distance2D(intersectionPoint) > currentPoint->startPoint.distance2D(currentPoint->directionPoint)
	//&& nextPoint->startPoint.distance2D(intersectionPoint) > nextPoint->startPoint.distance2D(nextPoint->directionPoint)

	realDistance2 < realDistance
	)
	{
	firstSmallest = currentPoint;
	secondSmallest = previousPoint;
	intersectionPointSmallest = intersectionPoint;
	smallestHeight = intersectionPoint.z;

	}









	*/













	/*


	intersectionPointSmallest.z = smallestHeight;

	auto previousFirstSmallest = (firstSmallest != roofData.begin()) ? std::prev(firstSmallest) : prev(roofData.end());
	auto nextSecondSmallest = (std::next(secondSmallest) != roofData.end()) ? std::next(secondSmallest) : roofData.begin();

	Line2D firstEaves(firstSmallest->startPoint, previousFirstSmallest->startPoint);
	Line2D secondEaves(secondSmallest->startPoint, nextSecondSmallest->startPoint);

	Point newIntersection = firstEaves.calcuateIntersectionPoint(secondEaves);
	newIntersection.z = _roof_level;

	float alpha = measureAngle(newIntersection, previousFirstSmallest->startPoint, nextSecondSmallest->startPoint) / 2;
	float beta = atan(tan(gamma) * sin(alpha));


	roofEdges.push_back({ firstSmallest->firstPoint, intersectionPointSmallest });
	roofEdges.push_back({ secondSmallest->firstPoint, intersectionPointSmallest });

	*firstSmallest = RoofData{ newIntersection , intersectionPointSmallest, true, intersectionPointSmallest, intersectionPointSmallest, beta };
	roofData.erase(secondSmallest);

	}



	*/




















































	//roofEdges.push_back(nextEdge[0]);
	//roofEdges.push_back(nextEdge[1]);


	/*

	float H = 0.01f;
	bool found = false;
	for (;H < 25.0f; H += 0.001f)
	{
	for (auto& data : roofData)
	{
	auto& firstPoint = std::get<0>(data);
	auto& secondPoint = std::get<1>(data);
	auto& beta = std::get<2>(data);

	float distance2d = firstPoint.distance(secondPoint);

	float newDist = H / tan(beta);

	Point newPoint;

	newPoint.x = firstPoint.x + (secondPoint.x - firstPoint.x) * newDist / distance2d;
	newPoint.y = firstPoint.y + (secondPoint.y - firstPoint.y) * newDist / distance2d;
	newPoint.z = _roof_level + H;

	roofEdges.push_back(firstPoint);
	roofEdges.push_back(newPoint);
	}

	for (size_t q = 1, limit = roofEdges.size() - 2; q < limit; q += 2)
	{
	if (roofEdges[q].distance(roofEdges[q + 2]) < 0.02f)
	{
	found = true;
	break;
	}
	}

	if (found)
	break;
	else
	roofEdges.clear();
	}

	*/
	

}