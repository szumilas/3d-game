#include "Car.h"

void Car::move()
{
	sin_rz = sin(rz);
	cos_rz = cos(rz);

	X += v * cos(rz) * 0.1;
	Y += v * sin(rz) * 0.1;
}

void Car::accelerate()
{
	v += 0.001;
	if (v > v_max)
		v = v_max;
}

void Car::slow()
{
	v -= 0.001;
	if (v < -v_max)
		v = -v_max;
}

void Car::turnRight()
{
	rz -= 0.005;
}

void Car::turnLeft()
{
	rz += 0.005;
}