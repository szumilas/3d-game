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
	void activateMovingXY() { if (previouslyRendered) previouslyRendered = false; else { moveOrbitXY = true; previousFlatCursor = flatCursor; } };
	void deactivateMovingXY() { moveOrbitXY = false; };
	void rotate() { rz += 3.14159265 / 360; };
	void changeAlpha() { if (alpha < 3.14 / 2) alpha += 3.14159265 / 360; else alpha = 3.14 / 36; };
	float getFlatCursorX() { return flatCursor.x; }
	float getFlatCursorY() { return flatCursor.y; }
	float getLookAtX() { return lookAtX; }
	float getLookAtY() { return lookAtY; }

	void calculateFlatCursorPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle);
	void calculateFlatCursorRealWorldPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle);

private:
	Point flatCursor;
	Point flatCursorRealWorld;
	Point previousFlatCursor;

public:


private:
	float lookAtX;
	float lookAtY;
	float distance;
	float rz;
	float alpha;

	bool moveOrbitXY;
	bool previouslyRendered;
};