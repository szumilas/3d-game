#include "Wheel.h"

Wheel::Wheel()
{

	X = 20;
	Y = -16;
	Z = 0.01;

	rz = 0;
}

void Wheel::adjustPosition(float Xcar, float Ycar, float rzCar, float xOffset, float yOffset, float steeringWheelAngle)
{
	float sin_rzCar = sin(rzCar);
	float cos_rzCar = cos(rzCar);

	rz = rzCar + steeringWheelAngle;

	X = Xcar + xOffset * cos_rzCar - yOffset * sin_rzCar;
	Y = Ycar + xOffset * sin_rzCar + yOffset * cos_rzCar;


}