#pragma once

#include "Object3D.h"
#include "GlobalStructures.h"

class Orbit : public Object3D
{
public:
	Orbit();
	Point getCameraCenter();
	Point getCameraLookAt();
	void zoomIn() { distance /= 1.05f; };
	void zoomOut() { distance *= 1.05f; };
	void activateMovingXY() { if (previouslyRendered) previouslyRendered = false; else { moveOrbitXY = true; previousFlatCursor = flatCursor; } };
	void deactivateMovingXY() { moveOrbitXY = false; };
	void rotate() { rz += 3.14159265f / 180.0f; };
	void changeAlpha() { if (alpha < 3.14 / 2) alpha += 3.14159265f / 180.0f; else alpha = 3.14f / 36.0f; };
	float getFlatCursorX() { return flatCursor.x; }
	float getFlatCursorY() { return flatCursor.y; }
	float getLookAtX() { return lookAtX; }
	float getLookAtY() { return lookAtY; }

	void calculateFlatCursorPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle);
	void calculateFlatCursorRealWorldPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle);

	void savePosition();

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