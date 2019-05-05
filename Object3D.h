#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <list>

#include "GlobalVariables.h"
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
	void importFromObjFile(const char* filePath, Textures textureName, float scaleRatio = 1.0f);
	void cretateGlobalVertex(const Point& localPoint);
	vector2D getGlobalVector(const vector2D& vLocal);

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

public:
	Point position;

	float rz;

	float sin_rz;
	float cos_rz;

};