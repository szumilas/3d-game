#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

int f()
{

	//glutGenBuffers()

	return 0;
}

class Mesh
{
public:

	struct Point2d
	{
		float x;
		float y;
	};

	struct Point3d
	{
		float x;
		float y;
		float z;
	};

	void init();


private:

public:

	std::vector<Point3d> vertices;
	std::vector<unsigned int> indices;
	std::vector<Point2d> textureCoordinates;


private:



};