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
	virtual void display();

private:


public:

	bool alreadyPrinted = false;

	struct Polygon
	{
		std::vector<Point> points;
		std::vector<Point> texturePoints;
		unsigned int noOfPoints;
		unsigned int idTexture;
		Color color;
		Color additionalColor;
	};


	std::vector<Polygon> polygons;


protected:
		Point position;

public:
	float X;
	float Y;
	float Z;

	float rz;

	float sin_rz;
	float cos_rz;

};