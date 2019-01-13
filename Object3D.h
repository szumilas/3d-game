#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <list>

#include "GlobalStructures.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Object3D
{
public:
	Object3D();
	void loadModel();
	void printModel();
	virtual void display();

private:


public:


protected:

	struct MeshTriangle
	{
		int p1;
		int p2;
		int p3;

		float red;
		float green;
		float blue;
	};

	struct Polygon
	{
		std::vector<Point> points;
		std::vector<Point> texturePoints;
		unsigned int noOfPoints;
		unsigned int idTexture;
		Color color;
		Color additionalColor;
	};

	Point position;
	std::vector<Point> mesh;
	std::vector<MeshTriangle> triangles;
	std::vector<Polygon> polygons;

	float X;
	float Y;
	float Z;

	float rz;

	float sin_rz;
	float cos_rz;

};