#pragma once

#include "Object3D.h"
#include "GlobalStructures.h"
#include "ObjectWithCamera.h"

class Orbit : public Object3D, public ObjectWithCamera
{
public:
	Orbit();
	Point getCameraCenter();
	Point getCameraLookAt();
	void zoomIn() { distance /= 1.10f; };
	void zoomOut() { distance *= 1.10f; };
	void activateMovingXY() { if (previouslyRendered) previouslyRendered = false; else { moveOrbitXY = true; previousFlatCursor = flatCursor; } };
	void deactivateMovingXY() { moveOrbitXY = false; };
	void rotate(int direction = 1) { rz += direction * 3.14159265f / 180.0f; };
	void changeAlpha() { if (alpha < 3.14 / 2) alpha += 3.14159265f / 180.0f; else alpha = 3.14f / 36.0f; };
	void BirdsEyeView() { alpha = 3.1 / 2; };
	float getFlatCursorX() { return flatCursor.x; }
	float getFlatCursorY() { return flatCursor.y; }
	Point getFlatCursor() { return flatCursor; }
	float getLookAtX() { return lookAtX; }
	float getLookAtY() { return lookAtY; }
	void displayFlatCursor();

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