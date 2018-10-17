#pragma once

#include "Object3D.h"
#include "GlobalStructures.h"

class Orbit : public Object3D
{
public:
	Orbit();
	Point getCameraCenter();
	Point getCameraLookAt();
	void zoomIn() { distance--; };
	void zoomOut() { distance++; };
	void rotate() { rz += 3.14159265 / 360; };
	void changeAlpha() { if (alpha < 3.14 / 2) alpha += 3.14159265 / 360; else alpha = 3.14 / 36; };

private:


public:


private:
	float lookAtX;
	float lookAtY;
	float distance;
	float rz;
	float alpha;



};