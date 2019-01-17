#include "Tree.h"

Tree::Tree(long long ref)
{
	refs.push_back(ref);
	_height = 4.0f;
}

void Tree::display()
{
	glColor3f(0.2f, 0.2f, 0.2f);

	for (size_t q = 0; q < points.size() - 1; q++)
	{
		Point& point = points[q];
		Point& nextPoint = points[q + 1];

		glBegin(GL_POLYGON);
		glVertex3f(point.x, point.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _min_height);
		glVertex3f(nextPoint.x, nextPoint.y, _height);
		glVertex3f(point.x, point.y, _height);
		glEnd();
	}

	glLineWidth(2.0f);
	glColor3f(0.0f, 0.8f, 0.2f);

	for (auto& polygon : polygons)
	{
		glBegin(GL_LINE_LOOP);
		for (auto& point : polygon.points)
		{
			glVertex3f(point.x, point.y, point.z);
		}
		glEnd();
	}
	glLineWidth(1.0f);

	/*glColor3f(0.1f, 0.6f, 0.1f);

	for (int q = 0; q < treeCrownPoints.size() - 2; q++)
	{
		glBegin(GL_POLYGON);
		glVertex3f(treeCrownPoints[q].x, treeCrownPoints[q].y, treeCrownPoints[q].z);
		glVertex3f(treeCrownPoints[q + 1].x, treeCrownPoints[q + 1].y, treeCrownPoints[q + 1].z);
		glVertex3f(treeCrownPoints[q + 2].x, treeCrownPoints[q + 2].y, treeCrownPoints[q + 2].z);
		glEnd();
	}*/

	Object3D::display();
}

void Tree::calculateXYfromRef(const std::map<long long, node> &nodes)
{
	Point newPoint;
	float size = 0.3;

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY - size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX - size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);

	newPoint.x = nodes.at(refs[0]).posX + size / 2;
	newPoint.y = nodes.at(refs[0]).posY + size / 2;
	points.push_back(newPoint);



	treeCrownPoints[0]  = Point{-0.26286500f,  0.00000000f,  0.42532500f};
	treeCrownPoints[1]  = Point{ 0.26286500f,  0.00000000f,  0.42532500f};
	treeCrownPoints[2]  = Point{-0.26286500f,  0.00000000f, -0.42532500f};
	treeCrownPoints[3]  = Point{ 0.26286500f,  0.00000000f, -0.42532500f};
	treeCrownPoints[4]  = Point{ 0.00000000f,  0.42532500f,  0.26286500f};
	treeCrownPoints[5]  = Point{ 0.00000000f,  0.42532500f, -0.26286500f};
	treeCrownPoints[6]  = Point{ 0.00000000f, -0.42532500f,  0.26286500f};
	treeCrownPoints[7]  = Point{ 0.00000000f, -0.42532500f, -0.26286500f};
	treeCrownPoints[8]  = Point{ 0.42532500f,  0.26286500f,  0.00000000f};
	treeCrownPoints[9]  = Point{-0.42532500f,  0.26286500f,  0.00000000f};
	treeCrownPoints[10] = Point{ 0.42532500f, -0.26286500f,  0.00000000f};
	treeCrownPoints[11] = Point{-0.42532500f, -0.26286500f,  0.00000000f};


	for (auto& treeCrownPoint : treeCrownPoints)
	{
		treeCrownPoint.x *= 6;
		treeCrownPoint.y *= 6;
		treeCrownPoint.z *= 7;
		treeCrownPoint.z += 3;
	}

	for (auto& treeCrownPoint : treeCrownPoints)
	{
		treeCrownPoint.x += nodes.at(refs[0]).posX;
		treeCrownPoint.y += nodes.at(refs[0]).posY;
		treeCrownPoint.z += _height;
	}

	polygons.push_back(Polygon{ { treeCrownPoints[0], treeCrownPoints[1], treeCrownPoints[4] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[0], treeCrownPoints[1], treeCrownPoints[6] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[0], treeCrownPoints[4], treeCrownPoints[9] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[0], treeCrownPoints[6], treeCrownPoints[11] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[0], treeCrownPoints[9], treeCrownPoints[11] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[1], treeCrownPoints[4], treeCrownPoints[8] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[1], treeCrownPoints[6], treeCrownPoints[10] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[1], treeCrownPoints[8], treeCrownPoints[10] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });

	polygons.push_back(Polygon{ { treeCrownPoints[2], treeCrownPoints[3], treeCrownPoints[5] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[2], treeCrownPoints[3], treeCrownPoints[7] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[2], treeCrownPoints[5], treeCrownPoints[9] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[2], treeCrownPoints[7], treeCrownPoints[11] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[2], treeCrownPoints[9], treeCrownPoints[11] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[3], treeCrownPoints[5], treeCrownPoints[8] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[3], treeCrownPoints[7], treeCrownPoints[10] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[3], treeCrownPoints[8], treeCrownPoints[10] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });

	polygons.push_back(Polygon{ { treeCrownPoints[4], treeCrownPoints[5], treeCrownPoints[8] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[4], treeCrownPoints[5], treeCrownPoints[9] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[6], treeCrownPoints[7], treeCrownPoints[10] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });
	polygons.push_back(Polygon{ { treeCrownPoints[6], treeCrownPoints[7], treeCrownPoints[11] },{ { 0,0 },{ 0,0 },{ 0,0 } }, 3 });

	for (auto& polygon : polygons)
	{
		polygon.color = Color{0,1,0};
	}
}