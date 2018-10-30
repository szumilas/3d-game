#include "Roof.h"

#include "PointInsidePolygonDetector.h"




float measureAngle(Point& currentPoint, Point& nextPoint, Point& previousPoint)
{
	float lenA = currentPoint.distance2D(nextPoint);
	float lenB = currentPoint.distance2D(previousPoint);
	float lenC = nextPoint.distance2D(previousPoint);

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
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(_red, _green, _blue);
	for (size_t q = 0; q < points.size() - 1; q++)
	{
		//auto point = points[q];
		//auto nextPoint = points[q + 1];
		
		glVertex3f(points[q].x, points[q].y, _roof_level);
		
	}
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(2.0f);
	glColor3f(0, 1.0f, 0);

	glBegin(GL_LINES);
	glVertex3f(points[0].x, points[0].y, _roof_level);
	glVertex3f(points[0].x, points[0].y, _roof_level + 5);
	glEnd();

/*
	for (size_t q = 0, limit = specialPoints.size() - 1; q < limit; q+=2)
	{
		glBegin(GL_LINES);
		glVertex3f(specialPoints[q].x, specialPoints[q].y, specialPoints[q].z);
		glVertex3f(specialPoints[q + 1].x, specialPoints[q + 1].y, specialPoints[q + 1].z);
		glEnd();
	}*/

	glLineWidth(2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (size_t q = 0, limit = roofEdges.size(); q < limit; q++)
	{
		glBegin(GL_LINES);
		glVertex3f(roofEdges[q].first.x, roofEdges[q].first.y, roofEdges[q].first.z);
		glVertex3f(roofEdges[q].second.x, roofEdges[q].second.y, roofEdges[q].second.z);
		glEnd();
	}
	glLineWidth(1.0f);


	glPointSize(10.0f);
	
	glBegin(GL_POINTS);

	for (size_t q = 0, limit = specPoints.size(); q < limit; q++)
	{
		glColor3f(std::get<1>(specPoints[q]), std::get<2>(specPoints[q]), std::get<3>(specPoints[q]));
		glVertex3f(std::get<0>(specPoints[q]).x, std::get<0>(specPoints[q]).y, std::get<0>(specPoints[q]).z);
	}
		glEnd();
	glPointSize(1.0f);

}

void Roof::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	MapObject::calculateXYfromRef(nodes);

	auto id = getId();
	//skip = true;
	/*if (id == 101192162 || id == 101202189 || id == 101192162 || id == 101209638 || id == 101209921 || id == 101215614)
	{
		if (id == 101215614)
			skip = true;
	}
	else */if(!skip)
	{
		try
		{
			float gamma = 30 / 180.0 * 3.14;


			for (size_t q = 0; q < points.size() - 1; q++)
			{
				Point& currentPoint = points[q];

				Point& nextPoint = points[q + 1];
				Point& previousPoint = points[(q - 1) % (points.size())];

				float alpha = measureAngle(currentPoint, previousPoint, nextPoint) / 2;
				float beta = atan(tan(gamma) * sin(alpha));

				Line2D firstLine(currentPoint, nextPoint);
				Line2D secondLine(currentPoint, previousPoint);

				Line2D firstBisector;
				Line2D secondBisector;

				firstBisector.calculateBisector(firstLine, secondLine);
				secondBisector.calculateBisector(firstLine, secondLine, true);


				float r = 0.05f;

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
						break;
					}
				}
			}

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

							/*if (l1.isSame(l2))
							{
								roofData.erase(it2);
								it = roofData.begin();
							}*/

							if (l.pointDistance(p2) < 0.05)
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


			while (roofData.size() > 12)
			{
				stopIteration:

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

					if (currentPoint->startPoint.distance2D(nextPoint->startPoint) < 0.05)
					{
						//specPoints.push_back( { currentPoint->startPoint, 1.0f, 0.5f, 1.0f } );

						roofEdges.push_back({ currentPoint->directionPoint, nextPoint->directionPoint });

						for (auto& w : waitingPoint)
						{
							if ((*w).startPoint == currentPoint->startPoint || (*w).startPoint == nextPoint->startPoint)
							{
								waitingPoint.clear();
								break;
							}
						}



						roofData.erase(currentPoint);
						roofData.erase(nextPoint);
						goto stopIteration;
					}

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


					if (roofData.size() == 13)
					{
						int h = 5;
						specPoints.push_back({ intersectionPoint, 1.0f, 1.0f, 0.5f });
						specPoints.push_back({ (*(waitingPoint.front())).startPoint, 0.3f, 1.0f, 1.0f });
					}

					/*if (q == 15 && c == 6)
					{
						specPoints.push_back({ currentPoint->startPoint, 1.0f, 0.5f, 0.0f });
						specPoints.push_back({ nextPoint->startPoint, 0.0f, 0.5f, 0.0f });
						specPoints.push_back({ intersectionPoint, 0.0f, 0.9f, 0.0f });
					}*/

					if (intersectionPoint.z > H && intersectionPoint.z < smallestHeight)
					{

						bool currentPointBisectionIsRising = currentPoint->startPoint.distance2D(intersectionPoint) > currentPoint->startPoint.distance2D(currentPoint->directionPoint);
						bool nextPointBisectionIsRising = nextPoint->startPoint.distance2D(intersectionPoint) > nextPoint->startPoint.distance2D(nextPoint->directionPoint);

						if (currentPointBisectionIsRising && nextPointBisectionIsRising)
						{
							firstSmallest = currentPoint;
							secondSmallest = nextPoint;
							intersectionPointSmallest = intersectionPoint;
							//nextEdge[0] = { std::get<0>(*currentPoint), intersectionPoint };
							//nextEdge[1] = { std::get<0>(*nextPoint), intersectionPoint };
							smallestHeight = intersectionPoint.z;
						}
						if (!currentPointBisectionIsRising)
						{
							bool found = false;

							if (waitingPoint.size() == 3 && c == 12 && specPoints.size() == 60)
							{
								int gg = 5;
							}

							for (auto& w : waitingPoint)
							{
								if ((*w).startPoint == currentPoint->startPoint)
								{
									found = true;
									break;
								}
							}

							if (!found)
								waitingPoint.push_back(currentPoint);

						}
						if (!nextPointBisectionIsRising)
						{
							bool found = false;
							for (auto& w : waitingPoint)
							{
								if ((*w).startPoint == nextPoint->startPoint)
								{
									found = true;
									break;
								}
							}

							if (!found)
								waitingPoint.push_back(nextPoint);
						}
					}
					int h = 5;


				}

				/*auto minHwaitingPoints = std::min_element(waitingPoint.begin(), waitingPoint.end(), [](RoofData& a, RoofData& b) {
					return a.firstPoint.z < b.firstPoint.z;
				});*/
				//*minHwaitingPoints


				if (!waitingPoint.empty() && (*waitingPoint.begin())->firstPoint.z < smallestHeight)
				{
					/*specPoints.push_back({ firstSmallest->firstPoint, 1.0f, 1.0f, 0.0f });
					specPoints.push_back({ secondSmallest->firstPoint, 1.0f, 0.0f, 1.0f });
					specPoints.push_back({ minHwaitingPoints->firstPoint, 1.0f, 0.0f, 0.0f });
					break;*/

					float z;

					{
						currentPoint = waitingPoint.front();
						nextPoint = (std::next(currentPoint) != roofData.end()) ? std::next(currentPoint) : roofData.begin();

						/*if (q == 15)
						{
							specPoints.push_back({ currentPoint->startPoint, 1.0f, 0.0f, 0.0f });
							specPoints.push_back({ nextPoint->startPoint, 1.0f, 0.0f, 0.0f });
						}*/


						Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
						Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

						Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

						bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

						intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);

						/*if (q == 15)
						{
							specPoints.push_back({ intersectionPoint, 1.0f, 0.5f, 0.5f });
						}*/

						z = intersectionPoint.z;

						firstSmallest = currentPoint;
						secondSmallest = nextPoint;

						intersectionPointSmallest = intersectionPoint;
					}
					{
						currentPoint = (currentPoint != roofData.begin()) ? std::prev(currentPoint) : prev(roofData.end());
						nextPoint = (std::next(currentPoint) != roofData.end()) ? std::next(currentPoint) : roofData.begin();

						/*if (q == 15)
						{
							specPoints.push_back({ currentPoint->startPoint, 0.0f, 1.0f, 0.0f });
							specPoints.push_back({ nextPoint->startPoint, 0.0f, 1.0f, 0.0f });
						}*/

						Line2D firstBisection(currentPoint->startPoint, currentPoint->directionPoint);
						Line2D secondBisection(nextPoint->startPoint, nextPoint->directionPoint);

						Point intersectionPoint = firstBisection.calcuateIntersectionPoint(secondBisection);

						bool direction = (currentPoint->directionPoint.x - currentPoint->startPoint.x) * (intersectionPoint.x - currentPoint->startPoint.x) > 0 ? true : false;

						intersectionPoint.z = H + (tan(currentPoint->beta) * intersectionPoint.distance2D(currentPoint->startPoint)) * (direction * 2 - 1);

						/*if (q == 15)
						{
							specPoints.push_back({ intersectionPoint, 1.0f, 0.5f, 0.5f });
						}*/

						if (z < intersectionPoint.z)
						{
							firstSmallest = currentPoint;
							secondSmallest = nextPoint;
							intersectionPointSmallest = intersectionPoint;
							z = intersectionPoint.z;
						}
					}


					//firstSmallest = currentPoint;
					//secondSmallest = nextPoint;

					intersectionPointSmallest.z = z;

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

					waitingPoint.clear();

					//break;

				}
				else
				{


					intersectionPointSmallest.z = smallestHeight;

					/*if (q == 15)
					{
						int h = 5;
						//specPoints.push_back({ intersectionPointSmallest , 1.0f, 1.0f, 0.0f });
						//specPoints.push_back({ firstSmallest->startPoint , 1.0f, 1.0f, 1.0f });
						//specPoints.push_back({ firstSmallest->directionPoint , 0.8f, 0.8f, 0.8f });
						//
						//specPoints.push_back({ secondSmallest->startPoint , 1.0f, 0.0f, 1.0f });
						//specPoints.push_back({ secondSmallest->directionPoint , 0.8f, 0.0f, 0.8f });
					}*/

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

				//*firstSmallest = std::make_tuple( newIntersection , intersectionPointSmallest , beta, false );
				//roofData.erase(secondSmallest);
			}
			if (roofData.size() == 2)
			{
				roofEdges.push_back({ roofData.front().firstPoint, roofData.back().firstPoint });

			}


			for (auto it = roofData.begin(); it != roofData.end(); it++)
			{
				specPoints.push_back({ it->startPoint, 0.8f, 0.1f, 0.7f });
				specPoints.push_back({ it->directionPoint, 0.2f, 0.1f, 0.7f });
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
		catch (...)
		{

		}
	}

}