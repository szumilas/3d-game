#include "Orbit.h"

Orbit::Orbit()
{
	lookAtX = 400;
	lookAtY = 400;
	distance = 50;
	rz = 3.14;
	alpha = 3.14 / 4;
}

Point Orbit::getCameraCenter()
{
	Point lookAtPoint;
	lookAtPoint.x = lookAtX + (distance * cos(alpha) * cos(rz));
	lookAtPoint.y = lookAtY + (distance * cos(alpha) * sin(rz));
	lookAtPoint.z = distance * sin(alpha);
	return lookAtPoint;

}

Point Orbit::getCameraLookAt()
{
	Point lookAtPoint;
	lookAtPoint.x = lookAtX;
	lookAtPoint.y = lookAtY;
	lookAtPoint.z = 0;
	return lookAtPoint;

}