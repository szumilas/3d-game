#include "Car.h"

Car::Car()
{
	X = 0;
	Y = 0;
	Z = 0.01;

	rz = 3.14 / 4 * 1;

	steeringWheelAngle = 0;

	wheelBase = 2.48;
	wheelBaseOffset = 0.04;
	track = 1.42;

	wheels.resize(4);

	for (auto& wheel : wheels)
	{
		wheel.loadModel();
	}

	cameraCenter = Point{-8, 0, 5};
	cameraLookAt = Point{0, 0, 3};

	resistanceRatio = (engine.getMaxTorque() * gearBox.getTopTransmission() * gearBox.getMainTransmission() * nm) / (rd * vMax * vMax);
}

void Car::move()
{
	engine.accelerator(tryAccelerate);

	float drivingForce = engine.getCurrentTorque() * gearBox.getCurrentTransmission() * gearBox.getMainTransmission() / rd * nm;
	float drivingResistance = resistanceRatio * v * v;

	if (v > 40)
		int h = 5;

	acceleration = (drivingForce - drivingResistance) / mass;

	/*if (tryAccelerate)
	{
		v += acceleration / FPS;
		if (v > v_max)
			v = v_max;
	}

	if (trySlow)
	{
		v -= acceleration / FPS;
		if (v < -v_max)
			v = -v_max;
	}*/

	v += acceleration / FPS;

	sin_rz = sin(rz);
	cos_rz = cos(rz);

	//local
	float vx = cos(steeringWheelAngle) * v / FPS;
	float vy = sin(steeringWheelAngle) * v / FPS;

	X += vx * cos(rz);
	X -= vy * sin(rz);

	Y += vy * cos(rz);
	Y += vx * sin(rz);

	rz += vy;

	if (v > -0.01 && v < 0.01 && !tryAccelerate)
		v = 0;

	if (v != 0)
	{
		if (steeringWheelAngle > 0)
			steeringWheelAngle -= 0.002;
		if (steeringWheelAngle < 0)
			steeringWheelAngle += 0.002;

		if (v > 0)
			v -= 0.05;
		if (v < 0)
			v += 0.05;
	}

	wheels[0].adjustPosition(X, Y, rz, wheelBase / 2 + wheelBaseOffset, track / 2, steeringWheelAngle);
	wheels[1].adjustPosition(X, Y, rz, wheelBase / 2 + wheelBaseOffset, -track / 2, steeringWheelAngle);
	wheels[2].adjustPosition(X, Y, rz, -wheelBase / 2 + wheelBaseOffset, track / 2);
	wheels[3].adjustPosition(X, Y, rz, -wheelBase / 2 + wheelBaseOffset, -track / 2);

	tryAccelerate = false;
	trySlow = false;
}

void Car::accelerate()
{
	tryAccelerate = true;
}

void Car::slow()
{
	trySlow = true;
}

void Car::turnRight()
{
	//rz -= 0.005;
	steeringWheelAngle -= 0.004;
	if (steeringWheelAngle < -3.14 / 6)
		steeringWheelAngle = -3.14 / 6;
}

void Car::turnLeft()
{
	//rz += 0.005;
	steeringWheelAngle += 0.004;
	if (steeringWheelAngle > 3.14 / 6)
		steeringWheelAngle = 3.14 / 6;
}

Point Car::getCameraCenter()
{

	return Point{ X + cameraCenter.x * cos(rz) - cameraCenter.y * sin(rz), Y + cameraCenter.x * sin(rz) + cameraCenter.y * cos(rz), cameraCenter.z };

}

Point Car::getCameraLookAt()
{
	return Point{ X + cameraLookAt.x * cos(rz) - cameraLookAt.y * sin(rz), Y + cameraLookAt.x * sin(rz) + cameraLookAt.y * cos(rz), cameraLookAt.z };
}

void Car::display()
{
	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

		sin_rz = sin(rz);
		cos_rz = cos(rz);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		for (auto& polygon : polygons)
		{
			glBindTexture(GL_TEXTURE_2D, polygon.idTexture);

			glBegin(GL_POLYGON);
			glColor3f(polygon.color.red, polygon.color.green, polygon.color.blue);

			for (unsigned int i = 0; i < polygon.noOfPoints; i++)
			{
				glTexCoord2f(polygon.texturePoints[i].x, polygon.texturePoints[i].y);
				Point toPrint;
				toPrint.x = X + polygon.points[i].x * cos_rz - polygon.points[i].y * sin_rz;
				toPrint.y = Y + polygon.points[i].x * sin_rz + polygon.points[i].y * cos_rz;
				toPrint.z = Z + polygon.points[i].z;
				glVertex3f(toPrint.x, toPrint.y, toPrint.z);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}