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
	_roof_level = _height;

	_color.red = 0.7f;
	_color.green = 0.7f;
	_color.blue = 0.7f;

	building = "roof_generated";
};


void Roof::calculateFinalGeometry()
{
	for (auto& roofSurfaces : roofsSurfaces)
	{
		if (roofSurfaces.first == false)
		{
			for (auto& roofSurface : roofSurfaces.second)
			{
				Polygon newPolygon;

				for (auto& point : roofSurface.points)
				{
					newPolygon.points.push_back({ point.x, point.y, point.z });
					newPolygon.texturePoints.push_back({ point.x / TextureManager::Instance()->textures[static_cast<long>(Textures::roof)].realWidth, point.y / TextureManager::Instance()->textures[static_cast<long>(Textures::roof)].realHeight });
				}

				newPolygon.noOfPoints = newPolygon.texturePoints.size();
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::roof_asphalt)].idTexture;
				newPolygon.color = Color{ 0.7f, 0.7f, 0.7f };

				newPolygon.additionalColor = Color{ 1.0f, 0.0f, 0.0f };

				polygons.push_back(newPolygon);
			}
		}
		else
		{
			for (auto& roofSurface : roofSurfaces.second)
			{
				Polygon newPolygon;

				auto& firstPoint = roofSurface.points[0];
				auto& lastPoint = roofSurface.points.back();

				vector2D v1(firstPoint, lastPoint);
				vector2D v2(Point(0.0f, 0.0f), Point(1.0f, 0.0f));
				auto angle = vector2D::realAngle(v1, v2);

				angle *= -1;

				for (auto& point : roofSurface.points)
				{
					newPolygon.points.push_back({ point.x, point.y, point.z });

					Point pointToBeRotated;
					Point rotatedPoint;
					pointToBeRotated.x = point.x - firstPoint.x;
					pointToBeRotated.y = point.y - firstPoint.y;

					rotatedPoint.x = pointToBeRotated.x * cos(angle) - pointToBeRotated.y * sin(angle);
					rotatedPoint.y = pointToBeRotated.x * sin(angle) + pointToBeRotated.y * cos(angle);

					newPolygon.texturePoints.push_back({ rotatedPoint.x / TextureManager::Instance()->textures[static_cast<long>(Textures::roof)].realWidth, rotatedPoint.y / TextureManager::Instance()->textures[static_cast<long>(Textures::roof)].realHeight });
				}

				newPolygon.noOfPoints = newPolygon.texturePoints.size();
				newPolygon.idTexture = TextureManager::Instance()->textures[static_cast<long>(Textures::roof)].idTexture;
				newPolygon.color = roofSurface.color;

				auto newColor = roofSurface.color.mixColor(Color{ 1.0f, 0.0f, 0.0f });
				newPolygon.additionalColor = roofSurface.color.mixColor(newColor);

				polygons.push_back(newPolygon);
			}
		}

	}
}

/*void Roof::display()
{
	bool printWavefrontLines = false;
	bool printLongRoofLines = false;
	bool printTriangles = false;
	bool printSpecialPoints = false;
	bool printRoofSurfaces = true;

	if (printRoofSurfaces)
	{
		for (auto& roofSurface : roofSurfaces)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, idTexture);
			glEnable(GL_TEXTURE_2D);

			glBegin(GL_POLYGON);
			if (isSelected)
			{
				auto newColor = roofSurface.color.mixColor(selectedColor);
				glColor3f(newColor.red, newColor.green, newColor.blue);
			}
			else
				glColor3f(roofSurface.color.red, roofSurface.color.green, roofSurface.color.blue);

			for (int q = 0; q < roofSurface.points.size(); q++)
			{
				glTexCoord2f(roofSurface.texturePoints[q].x, roofSurface.texturePoints[q].y);

				glVertex3f(roofSurface.points[q].x, roofSurface.points[q].y, roofSurface.points[q].z);
			}

			glEnd();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
	}

	if (printSpecialPoints)
	{
		glLineWidth(2.0f);
		glColor3f(0, 1.0f, 0);

		for (auto& specialPoint : specialPoints)
		{
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.5f, 0);
			glVertex3f(specialPoint.x, specialPoint.y, specialPoint.z);
			glVertex3f(specialPoint.x, specialPoint.y, specialPoint.z + 2.0f);
			glEnd();
		}

		glLineWidth(1.0f);
	}


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
	}

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
			auto& p2 = std::get<1>(wavefrontLines[q]);
			glVertex3f(static_cast<float>(p2.x), static_cast<float>(p2.y), static_cast<float>(p2.z));
			glEnd();
		}
		glPointSize(1.0f);
	}

}*/

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

	generateRoof(_roof_level, 30);
	if(_min_height)
		generateRoof(_min_height, 0);

}



void Roof::generateRoof(float roofStartLevel, float roofAngle)
{
	gamma = roofAngle / 180.0 * 3.14;

	roofPoints.clear();
	roofTriangleEdges.clear();
	triangles.clear();
	wavefront.clear();
	roofLines.clear();

	surfaces.clear();

	wavefrontLines.clear();
	longRoofLines.clear();
	specialPoints.clear();
	std::vector<RoofSurface> roofSurfaces;

	graphIdsSet.clear();
	graphIds.clear();
	connections.clear();


	for (size_t q = 0; q < points.size() - 1; q++)
	{
		RoofPoint roofPoint;
		roofPoint.id = q;
		roofPoint.point = points[q];
		roofPoint.point.z = roofStartLevel;

		roofPoints.push_back(roofPoint);
	}

	if (roofAngle == 0)
	{
		generateFlatRoof();
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
				roofPoint.point.z = roofStartLevel;

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

	if (
		roof_shape == "flat" ||

		getId() == 101191558 || getId() == 101202875
		|| getId() == 101206570 || getId() == 101207075
		|| getId() == 101215614 || getId() == 101215635
		|| getId() == 408013522 || getId() == 481858743
		|| getId() == 481858744 || getId() == 481858745
		|| getId() == 101208296 || getId() == 101215404

		|| getId() == 101215512 || getId() == 375365818

		|| getId() == 101140904 || getId() == 101194851
		|| getId() == 101206671 || getId() == 255868240
		|| getId() == 289721093 || getId() == 327031467
		|| getId() == 101124965 || getId() == 101125718
		|| getId() == 101188144 || getId() == 101193648
		|| getId() == 101201334 || getId() == 101134694
		|| getId() == 101139409 || getId() == 101140255
		|| getId() == 101143975 || getId() == 101205926
		|| getId() == 101207269 || getId() == 284166260
		|| getId() == 343661977 || getId() == 421145612
		|| getId() == 432328667 || getId() == 432937763
		)
	{
		generateFlatRoof();
	}
	else
	{
		int check = 1000;
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
						for (auto itWavefront = 0; itWavefront < wavefront[itWavefrontList].size(); itWavefront++)
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

					for (int it = 0; it < l2.size(); it++)
					{
						for (int itTrian = 0; itTrian < triangles.size(); itTrian++)
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
			check--;
			if (check < 0)
			{
				generateFlatRoof();
				return;
			}
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

		PointInsidePolygonDetector checker;
		for (auto& point : graphIdsSet)
		{
			if (!checker.isInside(points, Point(point)))
			{
				generateFlatRoof();
				return;
			}
		}

		for (auto& startPoint : roofPointsCopy)
		{
			graphIds.push_back({ startPoint.id, startPoint.point });
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

			if (!(it1->first <= lastRoofLevelId && it2->first <= lastRoofLevelId) && !(it1->first == it2->first))
				connections.push_back({ it1->first, it2->first });

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

			Color colorOfSurface{ _color.red, _color.green, _color.blue };
			vector2D wallLine(currentSurface[0], currentSurface.back());
			shadeTheWall(colorOfSurface, wallLine, 0.25f);

			roofSurfaces.push_back({ currentSurface, {}, colorOfSurface });
		}
	}

	polygonsCount = roofSurfaces.size();
	edgesCount = longRoofLines.size();


	for (auto& roofSurface : roofSurfaces)
	{
		float maxZ = -10000000.0f;
		float maxX = -10000000.0f;
		float minX = 10000000.0f;


		for (auto& point : roofSurface.points)
		{
			if (maxZ < point.z)
				maxZ = point.z;
			if (maxX < point.x)
				maxX = point.x;
			if (minX > point.x)
				minX = point.x;
		}

		std::vector<Point> roofSurfaceTexturePoins;
		auto deltaX = maxX - minX;
		auto detlaZ = maxZ - roofStartLevel;

		for (auto& point : roofSurface.points)
		{
			roofSurfaceTexturePoins.push_back({ (point.x - minX) / deltaX, (point.z - roofStartLevel) / detlaZ, 0 });
		}

		roofSurface.texturePoints = roofSurfaceTexturePoins;
	}

	roofsSurfaces.push_back({ true, roofSurfaces });
}

void Roof::generateFlatRoof()
{
	std::vector<RoofSurface> roofSurfaces;

	_color.red = 0.4f;
	_color.green = 0.4f;
	_color.blue = 0.4f;

	longRoofLines.clear();

	for (auto triangle : triangles)
	{
		std::vector<Point> roofSurface;
		roofSurface.push_back(getRoofPoint(triangle.idp1));
		roofSurface.push_back(getRoofPoint(triangle.idp2));
		roofSurface.push_back(getRoofPoint(triangle.idp3));
		roofSurfaces.push_back({ roofSurface, roofSurface, Color{ _color.red, _color.green, _color.blue } });
	}

	roofsSurfaces.push_back({ false, roofSurfaces });

	roof_shape = "flat";
}