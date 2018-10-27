#pragma once

#include <algorithm>
#include <vector>
#include <string>

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

	Point position;
	std::vector<Point> mesh;
	std::vector<MeshTriangle> triangles;

	float X;
	float Y;
	float Z;

	float rz;

	float sin_rz;
	float cos_rz;

};