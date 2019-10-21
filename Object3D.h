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
	void importFromObjFile(const char* filePath, Textures textureName, float& scaleRatio, float realLength = 0, std::map<std::string, Color>* materials = nullptr);
	std::map<std::string, Color> importMaterials(const char* filePath);
	void cretateGlobalVertex(const Point& localPoint);
	vector2D getGlobalVector(const vector2D& vLocal);
	vector2D getLocalVector(const vector2D& vGlobal);
	void setPosition(const Point& newPosition, float newRz) { position = newPosition, rz = newRz; }

protected:

	void myglDrawCircle(const Point& center, float r, int segments = 100);
	void myglDrawFillCircle(const Point& center, float r, int segments = 100);

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