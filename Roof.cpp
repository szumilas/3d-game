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
	bool printWavefrontLines = false;
	bool printLongRoofLines = true;
	bool printTriangles = false;
	bool printSpecialPoints = false;
	bool printRoofSurfaces = true;


	for (auto& roofSurface : roofSurfaces)
	{
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.2f, 0);
		for (auto& point : roofSurface)
		{
			glVertex3f(point.x, point.y, point.z);
		}
		glEnd();
	}



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

	
	if (printSpecialPoints)
	{
		for (auto& specialPoint : specialPoints)
		{
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.5f, 0);
			glVertex3f(specialPoint.x, specialPoint.y, specialPoint.z);
			glVertex3f(specialPoint.x, specialPoint.y, specialPoint.z + 2.0f);
			glEnd();
		}
	}

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




	/*glColor3f(0.5f, 1.0f, 0.0f);
	for (auto i = roofTriangleEdges.begin(); i != roofTriangleEdges.end(); i++)
	{
		glBegin(GL_LINES);
		glVertex3f(std::get<0>(*i).x, std::get<0>(*i).y, _roof_level);
		glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, _roof_level);
		glEnd();
	}*/

	glLineWidth(1.0f);

	if (printTriangles)
	{
		for (int i = 0; i < triangles.size(); i++)
		{
			glBegin(GL_LINE_LOOP);
			glColor3f(0.5f, 1.0f, 0.0f);
			auto p = getRoofPoint(triangles[i].idp1);
			glVertex3f(p.x, p.y, p.z);
			p = getRoofPoint(triangles[i].idp2);
			glVertex3f(p.x, p.y, p.z);
			p = getRoofPoint(triangles[i].idp3);
			glVertex3f(p.x, p.y, p.z);
			glEnd();
		}
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

	/*glLineWidth(3.0f);
	for (auto i = roofLines.begin(); i != roofLines.end(); i++)
	{
		glBegin(GL_LINES);
		glColor3f(0.2f, 0.2f, 0.4f);
		glVertex3f(i->first.x, i->first.y, i->first.z);
		glVertex3f(i->second.x, i->second.y, i->second.z);
		glEnd();
	}
	glLineWidth(1.0f);*/

	if (printLongRoofLines)
	{
		glLineWidth(3.0f);
		for (auto i = longRoofLines.begin(); i != longRoofLines.end(); i++)
		{
			glBegin(GL_LINES);
			glColor3f(0.2f, 0.2f, 0.4f);
			glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, std::get<1>(*i).z);
			glVertex3f(std::get<2>(*i).x, std::get<2>(*i).y, std::get<2>(*i).z);
			glEnd();
		}
		glLineWidth(1.0f);

		/*glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.4f);
		for (auto i = longRoofLines.begin(); i != longRoofLines.end(); i++)
		{
			glVertex3f(std::get<1>(*i).x, std::get<1>(*i).y, std::get<1>(*i).z);
			glVertex3f(std::get<2>(*i).x, std::get<2>(*i).y, std::get<2>(*i).z);
		}
		glEnd();
		glPointSize(1.0f);*/
	}

	/*for (auto i = 0; i < surfaces.size(); i++)
	{
		glBegin(GL_POLYGON);
		glColor3f(0.9f, 0.1f, 0.1f);
		glVertex3f(surfaces[i].p1.x, surfaces[i].p1.y, surfaces[i].p1.z);
		glVertex3f(surfaces[i].p2.x, surfaces[i].p2.y, surfaces[i].p2.z);
		glVertex3f(surfaces[i].p3.x, surfaces[i].p3.y, surfaces[i].p3.z);
		glVertex3f(surfaces[i].p4.x, surfaces[i].p4.y, surfaces[i].p4.z);
		glEnd();

		if (surfaces[i].p3.z < 15.0f)
		{
			surfaces[i].p3 = surfaces[i].p2;
		}
		if (surfaces[i].p4.z < 15.0f)
		{
			surfaces[i].p4 = surfaces[i].p1;
		}
	}*/


	if (printWavefrontLines)
	{
		glPointSize(5.0f);
		glColor3f(0.5f, 1.0f, 0.5f);
		for (size_t q = 0, limit = wavefrontLines.size(); q < limit; q++)
		{
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.5f, 0.7f);
			auto& p = std::get<0>(wavefrontLines[q]);
			glVertex3f(static_cast<float>(p.x), static_cast<float>(p.y), static_cast<float>(p.z));
			//glVertex3f(-2, 33, 15);
			auto& p2 = std::get<1>(wavefrontLines[q]);
			glVertex3f(static_cast<float>(p2.x), static_cast<float>(p2.y), static_cast<float>(p2.z));
			//glVertex3f(-5, 38, 15);
			glEnd();
		}
		glPointSize(1.0f);
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

bool Roof::anyPointInTriangle(long long idA, long long idB, long long idC)
{
	PointInsidePolygonDetector pointInsidePolygonDetector;

	std::vector<Point> triangle;
	triangle.push_back(getRoofPoint(idA));
	triangle.push_back(getRoofPoint(idB));
	triangle.push_back(getRoofPoint(idC));

	for(int i = nextIterator(idC, roofPoints); roofPoints[i].id != idA; i = (i + 1) % roofPoints.size())
	{
		bool result = pointInsidePolygonDetector.isInside(triangle, getRoofPoint(roofPoints[i].id));
		if (result)
			return true;
	}

	return false;
}


void Roof::calculateSpeedOfPoint(long long id)
{
	int currentWavefront = 0;
	int pointOnWavefront = 0;
	bool found = false;
	for (int itWavefrontList = 0; itWavefrontList < wavefront.size(); itWavefrontList++)
	{
		for (int itWavefront = 0; itWavefront < wavefront[itWavefrontList].size(); itWavefront++)
		{
			if (wavefront[itWavefrontList][itWavefront] == id)
			{
				currentWavefront = itWavefrontList;
				pointOnWavefront = itWavefront;
				found = true;
				break;
			}
		}
		if (found)
			break;
	}

	auto nextPointOnWavefront = (pointOnWavefront + 1 + wavefront[currentWavefront].size()) % wavefront[currentWavefront].size();
	auto previousPointOnWavefront = (pointOnWavefront - 1 + wavefront[currentWavefront].size()) % wavefront[currentWavefront].size();;

	calculateSpeedOfPoint(wavefront[currentWavefront][pointOnWavefront], wavefront[currentWavefront][nextPointOnWavefront], wavefront[currentWavefront][previousPointOnWavefront]);
}

void Roof::calculateSpeedOfPoint(long long IdcurrentPoint, long long IdnextPoint, long long IdprevPoint)
{
	Point currentPoint = getRoofPoint(IdcurrentPoint);
	Point nextPoint = getRoofPoint(IdnextPoint);
	Point prevPoint = getRoofPoint(IdprevPoint);


	vector2D vnext(currentPoint, nextPoint);
	vector2D vprev(currentPoint, prevPoint);

	vnext.convertIntoUnitVector();
	vprev.convertIntoUnitVector();

	vector2D vbisection(vnext, vprev);
	vbisection.convertIntoUnitVector();

	double alpha = measureAngle(currentPoint, prevPoint, nextPoint) / 2;
	double beta = atan(tan(gamma) * sin(alpha));

	if (vector2D::crossProduct(vbisection, vnext) < 0)
	{
		vbisection.negate();
	}

	vbisection.x /= tan(beta);
	vbisection.y /= tan(beta);

	RoofPoint newRoofPoint;
	newRoofPoint .point = getRoofPoint(IdcurrentPoint);
	newRoofPoint.id = IdcurrentPoint;
	newRoofPoint.dx = vbisection.x;
	newRoofPoint.dy = vbisection.y;

	updateRoofPoint(IdcurrentPoint, newRoofPoint);

}

double Roof::lineCollapseTime(RoofPoint& p1, RoofPoint& p2)
{
	double a = p1.point.x;
	double b = p1.dx;
	double c = p1.point.y;
	double d = p1.dy;
	double e = p2.point.x;
	double f = p2.dx;
	double g = p2.point.y;
	double h = p2.dy;

	double sqrtValue = (pow(2 * a * b - 2 * a * f - 2 * b * e + 2 * c * d - 2 * c * h - 2 * d * g + 2 * e * f + 2 * g * h, 2) - 4 * (a * a - 2 * a * e + c * c - 2 * c * g + e * e + g * g) * (b * b - 2 * b * f + d * d - 2 * d * h + f * f + h * h));
	if (sqrtValue < 0.0 && sqrtValue > -0.0001)
		sqrtValue = 0.0;

	return (sqrt(sqrtValue) - 2 * a * b + 2 * a * f + 2 * b * e - 2 * c * d + 2 * c * h + 2 * d * g - 2 * e * f - 2 * g * h) / (2 * (b *b - 2 * b * f + d * d - 2 * d * h + f * f + h * h));
}

double Roof::triangleCollapseTime(RoofPoint& p1, RoofPoint& p2, RoofPoint& p3)
{
	double a = p1.point.x;
	double b = p1.dx;
	double c = p1.point.y;
	double d = p1.dy;
	double e = p2.point.x;
	double f = p2.dx;
	double g = p2.point.y;
	double h = p2.dy;
	double i = p3.point.x;
	double j = p3.dx;
	double k = p3.point.y;
	double l = p3.dy;

	double s1 = ((a*(d - h) - a*(d - l) - c*(b - f) + c*(b - j) - k*(f - b) - g*(b - j) + e*(d - l) - i*(d - h)) + sqrt(pow(-a*(d - h) + a*(d - l) + c*(b - f) - c*(b - j) + k*(f - b) + g*(b - j) - e*(d - l) + i*(d - h), 2) - 4 * (a * g - a * k - c * e + c * i + e * k - g * i) * ((b - f) * (d - l) + (b - j) * (h - d)))) / (2 * ((b - f) * (d - l) + (b - j) * (h - d)));
	double s2 = ((a*(d - h) - a*(d - l) - c*(b - f) + c*(b - j) - k*(f - b) - g*(b - j) + e*(d - l) - i*(d - h)) - sqrt(pow(-a*(d - h) + a*(d - l) + c*(b - f) - c*(b - j) + k*(f - b) + g*(b - j) - e*(d - l) + i*(d - h), 2) - 4 * (a * g - a * k - c * e + c * i + e * k - g * i) * ((b - f) * (d - l) + (b - j) * (h - d)))) / (2 * ((b - f) * (d - l) + (b - j) * (h - d)));

	if (s1 > 0 && s2 > 0)
		return std::min(s1, s2);
	else
		return std::max(s1, s2);
}


void Roof::setCollisionPoint(long long& p1, long long& p2, long long& p3)
{
	auto roofPoint1 = getRoofPoint(p1);
	auto roofPoint2 = getRoofPoint(p2);
	auto roofPoint3 = getRoofPoint(p3);

	double dist12 = roofPoint1.distance2D(roofPoint2);
	double dist13 = roofPoint1.distance2D(roofPoint3);
	double dist23 = roofPoint2.distance2D(roofPoint3);

	double max = std::max(std::max(dist12, dist13), dist23);

	if (max == dist12)
	{
		std::swap(p1, p3);
		std::swap(p2, p3);
	}
	else if (max == dist13)
	{
		std::swap(p1, p2);
		std::swap(p2, p3);
	}
}

long long Roof::oppositeTriangleWithEdge(long long idp1, long long idp2, long long idp3)
{
	//edge p1-p2
	//triangle not containing p3
	for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
	{
		if ((idp1 == triangles[itTriangle].idp1 || idp1 == triangles[itTriangle].idp2 || idp1 == triangles[itTriangle].idp3) &&
			(idp2 == triangles[itTriangle].idp1 || idp2 == triangles[itTriangle].idp2 || idp2 == triangles[itTriangle].idp3))
		{
			if (!(idp3 == triangles[itTriangle].idp1 || idp3 == triangles[itTriangle].idp2 || idp3 == triangles[itTriangle].idp3))
			{
				if (triangles[itTriangle].idp1 != idp1 && triangles[itTriangle].idp1 != idp2)
					return triangles[itTriangle].idp1;
				if (triangles[itTriangle].idp2 != idp1 && triangles[itTriangle].idp2 != idp2)
					return triangles[itTriangle].idp2;
				if (triangles[itTriangle].idp3 != idp1 && triangles[itTriangle].idp3 != idp2)
					return triangles[itTriangle].idp3;
			}
		}
	}

	return -1;
}

void Roof::sortPoints(long long& idp1, long long& idp2, long long& idp3)
{
	long long first = std::min(std::min(idp1, idp2), idp3);
	long long second;
	long long third = std::max(std::max(idp1, idp2), idp3);

	if ((idp1 == first || idp1 == third) && (idp2 == first || idp2 == third))
	{
		second = idp3;
	}
	if ((idp1 == first || idp1 == third) && (idp3 == first || idp3 == third))
	{
		second = idp2;
	}
	if ((idp2 == first || idp2 == third) && (idp3 == first || idp3 == third))
	{
		second = idp1;
	}

	idp1 = first;
	idp2 = second;
	idp3 = third;

}

void Roof::removeTriangle(long long idp1, long long idp2, long long idp3)
{
	for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
	{
		if ((idp1 == triangles[itTriangle].idp1 || idp1 == triangles[itTriangle].idp2 || idp1 == triangles[itTriangle].idp3) &&
			(idp2 == triangles[itTriangle].idp1 || idp2 == triangles[itTriangle].idp2 || idp2 == triangles[itTriangle].idp3) &&
			(idp3 == triangles[itTriangle].idp1 || idp3 == triangles[itTriangle].idp2 || idp3 == triangles[itTriangle].idp3))
		{
			triangles.erase(triangles.begin() + itTriangle);
			break;
		}
	}
}

void Roof::removeTriangle(long long idp1, long long idp2)
{
	for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
	{
		if ((idp1 == triangles[itTriangle].idp1 || idp1 == triangles[itTriangle].idp2 || idp1 == triangles[itTriangle].idp3) &&
			(idp2 == triangles[itTriangle].idp1 || idp2 == triangles[itTriangle].idp2 || idp2 == triangles[itTriangle].idp3))
		{
			triangles.erase(triangles.begin() + itTriangle);
			break;
		}
	}
}

void Roof::renamePointInTriangles(long long oldIdP, long long newIdP)
{
	for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
	{
		if (oldIdP == triangles[itTriangle].idp1)
		{
			triangles[itTriangle].idp1 = newIdP;
		}
		if (oldIdP == triangles[itTriangle].idp2)
		{
			triangles[itTriangle].idp2 = newIdP;
		}
		if (oldIdP == triangles[itTriangle].idp3)
		{
			triangles[itTriangle].idp3 = newIdP;
		}
	}
}

void Roof::renamePointInSurfaces(long long oldIdP, long long newIdP)
{
	for (auto& surface : surfaces)
	{
		if (oldIdP == surface.idp1)
		{
			surface.idp1 = newIdP;
		}
		if (oldIdP == surface.idp2)
		{
			surface.idp2 = newIdP;
		}
	}
}

void Roof::removePointFromWavefront(long long id)
{
	for (int itWavefrontList = 0; itWavefrontList < wavefront.size(); itWavefrontList++)
	{
		for (int itWavefront = 0; itWavefront < wavefront[itWavefrontList].size(); itWavefront++)
		{
			if (wavefront[itWavefrontList][itWavefront] == id)
			{
				wavefront[itWavefrontList].erase(wavefront[itWavefrontList].begin() + itWavefront);
				break;
			}
		}
	}
}

double Roof::futureDistance(long long idp1, long long idp2, double h)
{
	auto p1 = getFullRoofPoint(idp1);
	auto p2 = getFullRoofPoint(idp2);

	auto futureP1 = p1.point;
	auto futureP2 = p2.point;

	futureP1.x += p1.dx * h;
	futureP1.y += p1.dy * h;
	futureP2.x += p2.dx * h;
	futureP2.y += p2.dy * h;

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
			closeLongRoofLine(id);
			roofPoints.erase(it);
			break;
		}
	}
}

void Roof::removeEmptyWavefronts()
{
	for (int itWavefrontList = 0; itWavefrontList < wavefront.size(); )
	{
		if (wavefront[itWavefrontList].empty())
			wavefront.erase(wavefront.begin() + itWavefrontList);
		else
			itWavefrontList++;
	}
}

void Roof::removeBrokenTriangles()
{
	//remove triangles with 2 points only
	for (int itTriangle = 0; itTriangle < triangles.size(); )
	{
		if (triangles[itTriangle].idp1 == triangles[itTriangle].idp2 ||
			triangles[itTriangle].idp1 == triangles[itTriangle].idp3 ||
			triangles[itTriangle].idp2 == triangles[itTriangle].idp3)
		{
			/*closeLongRoofLine(triangles[itTriangle].idp1);
			closeLongRoofLine(triangles[itTriangle].idp2);
			closeLongRoofLine(triangles[itTriangle].idp3);*/

			triangles.erase(triangles.begin() + itTriangle);
		}
		else
			itTriangle++;
	}

	//remove single traingles
	for (int itTriangle = 0; itTriangle < triangles.size(); )
	{
		int p1count = countTrianglesWithPoint(triangles[itTriangle].idp1);
		int p2count = countTrianglesWithPoint(triangles[itTriangle].idp2);
		int p3count = countTrianglesWithPoint(triangles[itTriangle].idp3);

		if ((p1count <= 1 && p2count <= 1) || (p1count <= 1 && p3count <= 1) || (p2count <= 1 && p3count <= 1))
		{
			auto heightOfCollision = lineCollapseTime(getFullRoofPoint(triangles[itTriangle].idp1), getFullRoofPoint(triangles[itTriangle].idp2));

			if (heightOfCollision > 0)
			{
				auto startPoint = getFullRoofPoint(triangles[itTriangle].idp1);

				Point vertex(startPoint.point.x + startPoint.dx * heightOfCollision, startPoint.point.y + startPoint.dy * heightOfCollision, startPoint.point.z + heightOfCollision);

				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp1), vertex });
				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp2), vertex });
				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp3), vertex });
			}
			else
			{
				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp1), getRoofPoint(triangles[itTriangle].idp2) });
				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp2), getRoofPoint(triangles[itTriangle].idp3) });
				longRoofLines.push_back({ -1, getRoofPoint(triangles[itTriangle].idp3), getRoofPoint(triangles[itTriangle].idp1) });
			}

			if (p1count <= 1)
			{
				removePointFromWavefront(triangles[itTriangle].idp1);
				removeRoofPoint(triangles[itTriangle].idp1);
			}
			if (p2count <= 1)
			{
				removePointFromWavefront(triangles[itTriangle].idp2);
				removeRoofPoint(triangles[itTriangle].idp2);
			}
			if (p3count <= 1)
			{
				removePointFromWavefront(triangles[itTriangle].idp3);
				removeRoofPoint(triangles[itTriangle].idp3);
			}

			triangles.erase(triangles.begin() + itTriangle);
		}
		else
			itTriangle++;

	}


}

int Roof::countTrianglesWithPoint(long long id)
{
	int result = 0;
	for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
	{
		if (triangles[itTriangle].idp1 == id || triangles[itTriangle].idp2 == id || triangles[itTriangle].idp3 == id)
			result++;
	}
	return result;
}

void Roof::openWavefrontSurfaces()
{
	for (auto& thisWaveFront : wavefront)
	{
		for (int q = 0; q < thisWaveFront.size(); q++)
		{
			Quadrangle newQuadrangle;
			newQuadrangle.closed = false;
			newQuadrangle.p1 = getRoofPoint(thisWaveFront[q]);
			newQuadrangle.p2 = getRoofPoint(thisWaveFront[(q + 1) % thisWaveFront.size()]);
			newQuadrangle.idp1 = thisWaveFront[q];
			newQuadrangle.idp2 = thisWaveFront[(q + 1) % thisWaveFront.size()];

			surfaces.push_back(newQuadrangle);


			wavefrontLines.push_back(std::make_tuple(getRoofPoint(thisWaveFront[q]), getRoofPoint(thisWaveFront[(q + 1) % thisWaveFront.size()]), 1));

		}
	}
}

void Roof::openLongRoofLines()
{
	for (auto& thisWaveFront : wavefront)
	{
		for (int q = 0; q < thisWaveFront.size(); q++)
		{
			longRoofLines.push_back(std::make_tuple(thisWaveFront[q], getRoofPoint(thisWaveFront[q]), Point()));
		}
	}
}

void Roof::createRoofLevelLongRoofLines()
{
	for (auto& thisWaveFront : wavefront)
	{
		for (int q = 0; q < thisWaveFront.size(); q++)
		{
			longRoofLines.push_back(std::make_tuple(-1, getRoofPoint(thisWaveFront[q]), getRoofPoint(thisWaveFront[(q + 1) % thisWaveFront.size()])));
		}
	}
}

void Roof::closeLongRoofLine(const long long& id)
{
	for (auto& longRoofLine : longRoofLines)
	{
		if (std::get<0>(longRoofLine) == id)
		{
			std::get<0>(longRoofLine) = -1;
			std::get<2>(longRoofLine) = getRoofPoint(id);
			break;
		}
	}
}

void Roof::restartLongRoofPoint(const long long& id)
{
	for (auto& longRoofLine : longRoofLines)
	{
		if (std::get<0>(longRoofLine) == id)
		{
			std::get<0>(longRoofLine) = -1;
			std::get<2>(longRoofLine) = getRoofPoint(id);
			longRoofLines.push_back(std::make_tuple(id, getRoofPoint(id), Point()));
			break;
		}
	}
}

void Roof::closeWavefrontSurfaces()
{
	for (auto& surface : surfaces)
	{
		if (surface.closed == false)
		{
			surface.p3 = getRoofPoint(surface.idp2);
			surface.p4 = getRoofPoint(surface.idp1);

			if (surface.p3.z < 15.0f)
			{
				int h = 5;
			}

			surface.closed = true;
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

	int firstPoint = 0;
	int secondPoint = 0;
	int thirdPoint = 0;

	int clockWiseCheckCount = 0;

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
			firstPoint = 0;
			roofPointsCopy = roofPoints;
		}

		secondPoint = nextIterator(roofPoints[firstPoint].id, roofPoints);
		thirdPoint = nextIterator(roofPoints[secondPoint].id, roofPoints);


		vector2D v1(roofPoints[firstPoint].point, roofPoints[secondPoint].point);
		vector2D v2(roofPoints[firstPoint].point, roofPoints[thirdPoint].point);

		if (!positiveAngle(v1, v2) && !anyPointInTriangle(roofPoints[firstPoint].id, roofPoints[secondPoint].id, roofPoints[thirdPoint].id))
		{
			Triangle triangle;
			triangle.idp1 = roofPoints[firstPoint].id;
			triangle.idp2 = roofPoints[secondPoint].id;
			triangle.idp3 = roofPoints[thirdPoint].id;
			triangles.push_back(triangle);

			roofPoints.erase(roofPoints.begin() + secondPoint);
			firstPoint = firstPoint % roofPoints.size();
			clockWiseCheckCount = 0;
		}
		else
		{
			firstPoint = nextIterator(roofPoints[firstPoint].id, roofPoints);
			clockWiseCheckCount++;
		}
	}

	firstPoint = 0;
	secondPoint = nextIterator(roofPoints[firstPoint].id, roofPoints);
	thirdPoint = nextIterator(roofPoints[secondPoint].id, roofPoints);

	Triangle triangle;
	triangle.idp1 = roofPoints[firstPoint].id;
	triangle.idp2 = roofPoints[secondPoint].id;
	triangle.idp3 = roofPoints[thirdPoint].id;
	triangles.push_back(triangle);

	roofPoints = roofPointsCopy;

	std::vector<long long> firstWavefront;
	for (int itRoofPoint = 0; itRoofPoint < roofPoints.size(); itRoofPoint++)
	{
		firstWavefront.push_back(roofPoints[itRoofPoint].id);
	}
	wavefront.push_back(firstWavefront);

	for (int itRoofPoint = 0; itRoofPoint < roofPoints.size(); itRoofPoint++)
	{
		calculateSpeedOfPoint(roofPoints[itRoofPoint].id);
	}

	openWavefrontSurfaces();
	openLongRoofLines();
	createRoofLevelLongRoofLines();

	specialPoints.push_back(getRoofPoint(0));
	//specialPoints.push_back(getRoofPoint(1));

	int breakbreak = 0;

	while (!wavefront.empty())
	{
		for (int itWavefrontList = 0; itWavefrontList < wavefront.size(); )
		{
			if (wavefront[itWavefrontList].size() == 3)
			{
				auto p1 = wavefront[itWavefrontList][0];
				auto p2 = wavefront[itWavefrontList][1];
				auto p3 = wavefront[itWavefrontList][2];
				removeTriangle(p1, p2, p3);
				removeRoofPoint(p1);
				removeRoofPoint(p2);
				removeRoofPoint(p3);
				wavefront.erase(wavefront.begin() + itWavefrontList);
				//registerTriangles();
			}
			else if (wavefront[itWavefrontList].size() == 2)
			{
				auto p1 = wavefront[itWavefrontList][0];
				auto p2 = wavefront[itWavefrontList][1];
				removeTriangle(p1, p2);
				removeRoofPoint(p1);
				removeRoofPoint(p2);
				wavefront.erase(wavefront.begin() + itWavefrontList);
			}
			else if (wavefront[itWavefrontList].size() == 1)
			{
				auto p1 = wavefront[itWavefrontList][0];
				removeRoofPoint(p1);
				wavefront.erase(wavefront.begin() + itWavefrontList);
			}
			else
				itWavefrontList++;
		}

		if (wavefront.empty())
			break;

		bool egdeEvent = true;

		double heightOfNextCollision = 100000.0f;
		long long edgeEventP1 = 0;
		long long edgeEventP2 = 0;

		long long flipSplitEventP1 = 0;
		long long flipSplitEventP2 = 0;
		long long flipSplitEventP3 = 0;

		for (int itTriangle = 0; itTriangle < triangles.size(); itTriangle++)
		{
			auto p1 = getFullRoofPoint(triangles[itTriangle].idp1);
			auto p2 = getFullRoofPoint(triangles[itTriangle].idp2);
			auto p3 = getFullRoofPoint(triangles[itTriangle].idp3);

			double joinTimeP1P2 = lineCollapseTime(p1, p2);
			if (joinTimeP1P2 < heightOfNextCollision && joinTimeP1P2 >= 0.0f)
			{
				egdeEvent = true;
				heightOfNextCollision = joinTimeP1P2;
				edgeEventP1 = p1.id;
				edgeEventP2 = p2.id;
			}

			double joinTimeP1P3 = lineCollapseTime(p1, p3);
			if (joinTimeP1P3 < heightOfNextCollision && joinTimeP1P3 >= 0.0f)
			{
				egdeEvent = true;
				heightOfNextCollision = joinTimeP1P3;
				edgeEventP1 = p1.id;
				edgeEventP2 = p3.id;
			}

			double joinTimeP2P3 = lineCollapseTime(p2, p3);
			if (joinTimeP2P3 < heightOfNextCollision && joinTimeP2P3 >= 0.0f)
			{
				egdeEvent = true;
				heightOfNextCollision = joinTimeP2P3;
				edgeEventP1 = p2.id;
				edgeEventP2 = p3.id;
			}

			double triangle123CollapseTime = triangleCollapseTime(p1, p2, p3);
			if (triangle123CollapseTime < heightOfNextCollision && triangle123CollapseTime >= 0.0f &&
				futureDistance(p1.id, p2.id, triangle123CollapseTime) > 0.0001f &&
				futureDistance(p2.id, p3.id, triangle123CollapseTime) > 0.0001f &&
				futureDistance(p1.id, p3.id, triangle123CollapseTime) > 0.0001f)
			{
				egdeEvent = false;
				heightOfNextCollision = triangle123CollapseTime;
				flipSplitEventP1 = p1.id;
				flipSplitEventP2 = p2.id;
				flipSplitEventP3 = p3.id;
			}
		}

		for (int itRoofPoint = 0; itRoofPoint < roofPoints.size(); itRoofPoint++)
		{
			auto nextPoint = getFullRoofPoint(roofPoints[itRoofPoint].id);

			Point next(roofPoints[itRoofPoint].point.x + heightOfNextCollision * roofPoints[itRoofPoint].dx, roofPoints[itRoofPoint].point.y + heightOfNextCollision * roofPoints[itRoofPoint].dy, roofPoints[itRoofPoint].point.z + heightOfNextCollision);
			roofLines.push_back({ roofPoints[itRoofPoint].point, next });


			nextPoint.point = next;
			updateRoofPoint(roofPoints[itRoofPoint].id, nextPoint);

			if (roofPoints[itRoofPoint].point.z < 15.0f)
			{
				int h = 5;
			}
		}

		closeWavefrontSurfaces();
		openWavefrontSurfaces();

		if (egdeEvent)
		{
			removeTriangle(edgeEventP1, edgeEventP2);
			updatePoint(edgeEventP2, getRoofPoint(edgeEventP1));

			removePointFromWavefront(edgeEventP2);
			renamePointInTriangles(edgeEventP2, edgeEventP1);
			renamePointInSurfaces(edgeEventP2, edgeEventP1);
			removeRoofPoint(edgeEventP2);
			restartLongRoofPoint(edgeEventP1);

			calculateSpeedOfPoint(edgeEventP1);

		}
		else
		{
			sortPoints(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

			setCollisionPoint(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

			auto oppositePoint = oppositeTriangleWithEdge(flipSplitEventP2, flipSplitEventP3, flipSplitEventP1);

			if (oppositePoint != -1) //flip event
			{
				removeTriangle(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);
				removeTriangle(oppositePoint, flipSplitEventP2, flipSplitEventP3);

				Triangle t1;
				t1.idp1 = flipSplitEventP1;
				t1.idp2 = flipSplitEventP2;
				t1.idp3 = oppositePoint;

				Triangle t2;
				t2.idp1 = flipSplitEventP1;
				t2.idp2 = flipSplitEventP3;
				t2.idp3 = oppositePoint;

				triangles.push_back(t1);
				triangles.push_back(t2);


			}
			else //split event
			{
				setCollisionPoint(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

				removeTriangle(flipSplitEventP1, flipSplitEventP2, flipSplitEventP3);

				RoofPoint newPoint = getFullRoofPoint(flipSplitEventP1);
				long long newId = nextRoofPointid();
				long long oldId = flipSplitEventP1;
				newPoint.id = newId;
				roofPoints.push_back(newPoint);

				closeLongRoofLine(oldId);
				longRoofLines.push_back(std::make_tuple(newId, getRoofPoint(newId), Point()));
				longRoofLines.push_back(std::make_tuple(oldId, getRoofPoint(newId), Point()));

				int currentWavefront = 0;
				int pointOnWavefront = 0;
				bool found = false;
				for (int itWavefrontList = 0; itWavefrontList < wavefront.size(); itWavefrontList++)
				{
					for (auto itWavefront = 0 ; itWavefront < wavefront[itWavefrontList].size(); itWavefront++)
					{
						if (wavefront[itWavefrontList][itWavefront] == flipSplitEventP1)
						{
							currentWavefront = itWavefrontList;
							pointOnWavefront = itWavefront;
							found = true;
							break;
						}
					}
					if (found)
						break;
				}

				std::vector<long long> l1;
				std::vector<long long> l2;

				auto nextInWavefront = pointOnWavefront;

				for (auto it = pointOnWavefront; wavefront[currentWavefront][it] != flipSplitEventP3; )
				{
					l1.push_back(wavefront[currentWavefront][it]);
					nextInWavefront = it;

					it++;
					it = it % wavefront[currentWavefront].size();
				}
				nextInWavefront++;
				nextInWavefront = nextInWavefront % wavefront[currentWavefront].size();

				l2.push_back(roofPoints[roofPoints.size() - 1].id);
				for (auto it = nextInWavefront; wavefront[currentWavefront][it] != wavefront[currentWavefront][pointOnWavefront]; )
				{
					l2.push_back(wavefront[currentWavefront][it]);
					it++;
					it = it % wavefront[currentWavefront].size();
				}

				wavefront.erase(wavefront.begin() + currentWavefront);
				wavefront.push_back(l1);
				wavefront.push_back(l2);

				calculateSpeedOfPoint(newPoint.id);
				calculateSpeedOfPoint(flipSplitEventP1);

				for(int it = 0; it < l2.size(); it++)
				{
					for(int itTrian = 0; itTrian < triangles.size(); itTrian++)
					{
						if (triangles[itTrian].idp1 == l2[it] || triangles[itTrian].idp2 == l2[it] || triangles[itTrian].idp3 == l2[it])
						{
							if (triangles[itTrian].idp1 == oldId)
							{
								triangles[itTrian].idp1 = newId;
							}
							if (triangles[itTrian].idp2 == oldId)
							{
								triangles[itTrian].idp2 = newId;
							}
							if (triangles[itTrian].idp3 == oldId)
							{
								triangles[itTrian].idp3 = newId;
							}
						}

					}
				}
			}
		}
		
		//if(breakbreak > 18)
		//	break;
		breakbreak++;
		removeBrokenTriangles();
		removeEmptyWavefronts();
	}

	closeWavefrontSurfaces();

	for (auto& longRoofLine : longRoofLines)
	{
		graphIdsSet.insert(std::get<1>(longRoofLine));
		graphIdsSet.insert(std::get<2>(longRoofLine));

	}

	for (auto it = graphIdsSet.begin(); it != graphIdsSet.end();)
	{
		bool removed = false;
		for (auto& startPoint : roofPointsCopy)
		{
			if (startPoint.point == *it)
			{
				it = graphIdsSet.erase(it);
				removed = true;
				break;
			}
		}
		if (!removed)
			it++;
	}

	for (auto& startPoint : roofPointsCopy)
	{
		graphIds.push_back({ startPoint.id, startPoint.point});
	}

	auto lastRoofLevelId = graphIds.back().first;
	auto nextId = lastRoofLevelId + 1;

	for (auto& point : graphIdsSet)
	{
		graphIds.push_back({ nextId, point });
		nextId++;
	}

	for (auto& longRoofLine : longRoofLines)
	{
		Point p1 = std::get<1>(longRoofLine);
		Point p2 = std::get<2>(longRoofLine);

		std::vector<std::pair<long long, Point>>::iterator it1 = std::find_if(graphIds.begin(), graphIds.end(), [p1](std::pair<long long, Point>& pair) { return pair.second == p1; });
		std::vector<std::pair<long long, Point>>::iterator it2 = std::find_if(graphIds.begin(), graphIds.end(), [p2](std::pair<long long, Point>& pair) { return pair.second == p2; });

		if(!(it1->first <= lastRoofLevelId && it2->first <= lastRoofLevelId) && !(it1->first == it2->first))
			connections.push_back({it1->first, it2->first});

	}

	std::vector<std::vector<long long>> neighbours;

	std::vector<std::pair<int, long long>> dataCopy; //pair<state, parent>

	for (auto& graphId : graphIds)
	{
		std::vector<long long> thisIdNeighbours;
		for (auto& connection : connections)
		{
			if (connection.first == graphId.first)
				thisIdNeighbours.push_back(connection.second);
			if (connection.second == graphId.first)
				thisIdNeighbours.push_back(connection.first);
		}
		neighbours.push_back(thisIdNeighbours);
		dataCopy.push_back({ 0, -1 });
	}


	for (long long idFrom = 0; idFrom < roofPointsCopy.size(); idFrom++)
	{
		std::vector<std::pair<int, long long>> data = dataCopy;

		long long idTo = (idFrom + 1) % roofPointsCopy.size();


		data[idFrom].first = 1;
		std::queue<long long> idsQueue;

		idsQueue.push(idFrom);

		while (!idsQueue.empty())
		{
			auto currentId = idsQueue.front();

			if (currentId == idTo)
				break;

			idsQueue.pop();
			for (auto& neighbour : neighbours[currentId])
			{
				if (data[neighbour].first == 0)
				{
					data[neighbour].first = 1;
					data[neighbour].second = currentId;
					idsQueue.push(neighbour);
				}
			}
			data[currentId].first = 2;
		}

		std::vector<long long> path;
		path.push_back(idTo);

		auto checkHistory = data[idTo].second;

		while (checkHistory != -1)
		{
			path.push_back(checkHistory);
			checkHistory = data[checkHistory].second;
		}

		std::vector<Point> currentSurface;

		for (auto& currentPoint : path)
		{
			currentSurface.push_back(graphIds[currentPoint].second);
		}

		roofSurfaces.push_back(currentSurface);
	}
}