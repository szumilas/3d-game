#define NOMINMAX


#include "Car.h"

#include <algorithm>

Car::Car(CarBrand carBrand, float startX, float startY, Point* globalCameraCenter, Point* globalCameraLookAt) : carBrand(carBrand), engine(carBrand), gearBox(carBrand)
{
	Car::globalCameraCenter = globalCameraCenter;
	Car::globalCameraLookAt = globalCameraLookAt;

	mass = carDB.at(carBrand).mass;
	vMax = carDB.at(carBrand).vMax / 3.6;

	position.x = startX -140;
	position.y = startY -347;
	position.z = 0.01;

	rz = 3.14 / 4 * 0 + 4.36;

	steeringWheelAngle = 0;

	wheels.resize(4);

	for (auto& wheel : wheels)
	{
		wheel.loadModel();
	}

	cameraCenter = Point{-8, 0, 5};
	//cameraCenter = Point{ -0.001, 0, 15 };
	cameraLookAt = Point{0, 0, 3};

	setLastWheelPosition();


	engineSound = Game::soundManager.registerSoundInstance(carDB.at(carBrand).engineSound);

}

void Car::move()
{
	//cameraCenter.y = sin(steeringWheelAngle) * 10.0;

	if (trySlow && vLoc.x > 1.0)
	{
		vLoc.x -= 1;
	}
	if (!tryAccelerate && vLoc.x <= 1.0)
	{
		vLoc.x = 0.0;
		vLoc.y = 0.0;
	}

	//Automatic Transmission
	//------------------------

	if(tryAccelerate && gearBox.getCurrentGear() == 0)
		gearBox.gearUp();
	else if (engine.nextGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getNextTransmission(), rd, v))
		gearBox.gearUp();
	else if (engine.previousGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getPreviousTransmission(), rd, v))
		gearBox.gearDown();

	//------------------------

	engine.setRPM(Engine::calculateRMP(gearBox.getCurrentTransmission() * gearBox.getMainTransmission(), rd, v));

	float drivingForce = 0.0f;
	float drivingResistance = 0.0f;

	if(tryAccelerate)
		drivingForce = engine.getCurrentTorque() * gearBox.getCurrentTransmission() * gearBox.getMainTransmission() / rd * nm;

	resistanceRatio = (engine.getMaxTorque() * gearBox.getCurrentTransmission() * gearBox.getMainTransmission() * nm) / (rd * vMax * vMax);

	drivingResistance = resistanceRatio * v * v;
	if (v > 0 && v < 15)
		drivingResistance = resistanceRatio * 15 * 15;
	
	acceleration = (drivingForce - drivingResistance) / mass;
	
	forces.clear();

	forces.push_back({ {frontWheelsXoffset, 0}, steeringWheelAngle, std::max((drivingForce - drivingResistance) / 2, 0.0f) });

	float forceVal = mass * vLoc.x * vLoc.x * tan(steeringWheelAngle) / (frontWheelsXoffset - backWheelsXoffset);

	forceVal = sgn(tan(steeringWheelAngle)) * std::min(abs(forceVal), mass * 9.81f * 0.8f);

	forces.push_back({ { frontWheelsXoffset, 0 }, vector2D(-forceVal * tan(steeringWheelAngle) / 2, forceVal / 2) });
	forces.push_back({ { backWheelsXoffset, 0 }, vector2D(0, forceVal / 2) });


	calculateMovement();

	std::vector<Force> globalForces;

	for (auto& force : forces)
	{
		globalForces.push_back({{ position.x + force.center.x * cos(rz) - force.center.y * sin(rz), position.y + force.center.x * sin(rz) + force.center.y * cos(rz) }, force.direction + rz, force.value});
	}

	forces = globalForces;
	

	straightenSteeringWheelAngle();
	calculateCarDrift();
	
	playEngineSound();

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
	steeringWheelAngle -= 0.06;

	/*if (steeringWheelAngle > -0.1 && steeringWheelAngle < 0.1)
		steeringWheelAngle -= 0.016;
	else if (steeringWheelAngle > 0)
		steeringWheelAngle /= 1.1f;
	else
		steeringWheelAngle *= 1.1f;*/

	if (steeringWheelAngle < -3.14 / 6)
		steeringWheelAngle = -3.14 / 6;
}

void Car::turnLeft()
{
	//rz += 0.005;
	steeringWheelAngle += 0.06;

	/*if (steeringWheelAngle > -0.1 && steeringWheelAngle < 0.1)
		steeringWheelAngle += 0.016;
	else if (steeringWheelAngle > 0)
		steeringWheelAngle *= 1.1f;
	else
		steeringWheelAngle /= 1.1f;*/

	if (steeringWheelAngle > 3.14 / 6)
		steeringWheelAngle = 3.14 / 6;
}

Point Car::getCameraCenter()
{

	return Point{ position.x + cameraCenter.x * cos(rz) - cameraCenter.y * sin(rz), position.y + cameraCenter.x * sin(rz) + cameraCenter.y * cos(rz), cameraCenter.z };

}

Point Car::getCameraLookAt()
{
	return Point{ position.x + cameraLookAt.x * cos(rz) - cameraLookAt.y * sin(rz), position.y + cameraLookAt.x * sin(rz) + cameraLookAt.y * cos(rz), cameraLookAt.z };
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
				toPrint.x = position.x + polygon.points[i].x * cos_rz - polygon.points[i].y * sin_rz;
				toPrint.y = position.y + polygon.points[i].x * sin_rz + polygon.points[i].y * cos_rz;
				toPrint.z = position.z + polygon.points[i].z;
				glVertex3f(toPrint.x, toPrint.y, toPrint.z);
			}
			glEnd();
		}

		leftWheel.rz = steeringWheelAngle;
		rightWheel.rz = steeringWheelAngle;

		for (auto& currentWheel : {&backWheels, &leftWheel, &rightWheel})
		{
			currentWheel->setCarPosition(position.x, position.y, position.z);
			currentWheel->setCarAngle(rz);

			currentWheel->rotate(v);
			currentWheel->display();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);


		for (auto& force : forces)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glLineWidth(5);
			glBegin(GL_LINES);

			glVertex3f(force.center.x, force.center.y, 0.0f);
			glVertex3f(force.center.x + force.value / 1000 * cos(force.direction), force.center.y + force.value / 1000 * sin(force.direction), 0.0f);

			glEnd();
			glLineWidth(1);
		}


		glColor3f(0.0f, 0.0f, 1.0f);
		glLineWidth(5);
		glBegin(GL_LINES);

		glVertex3f(position.x, position.y, 0.0f);
		glVertex3f(position.x + vx / 10, position.y + vy / 10, 0.0f);

		glEnd();
		glLineWidth(1);


		


		glColor3f(0.1f, 0.1f, 0.1f);
		glPointSize(5);
		glBegin(GL_POINTS);

		for (auto& p : wheelsTrace)
		{
			glVertex3f(p.x, p.y, 0.0f);
		}

		glEnd();
		glPointSize(1);

	}
}

void Car::importFromObjFile()
{
	Object3D::importFromObjFile((carDB.at(carBrand).objFilePath + "body.obj").c_str(), carDB.at(carBrand).textureName, 0.165);
	backWheels.importFromObjFile((carDB.at(carBrand).objFilePath + "back_wheels.obj").c_str(), carDB.at(carBrand).textureName, 0.165);
	backWheels.calculateGeometry(carDB.at(carBrand).backWheelsXoffset);
	leftWheel.importFromObjFile((carDB.at(carBrand).objFilePath + "left_wheel.obj").c_str(), carDB.at(carBrand).textureName, 0.165);
	leftWheel.calculateGeometry(carDB.at(carBrand).frontWheelsXoffset, carDB.at(carBrand).frontWheelsYoffset);
	rightWheel.importFromObjFile((carDB.at(carBrand).objFilePath + "right_wheel.obj").c_str(), carDB.at(carBrand).textureName, 0.165);
	rightWheel.calculateGeometry(carDB.at(carBrand).frontWheelsXoffset, -carDB.at(carBrand).frontWheelsYoffset);

	frontWheelsYoffset = carDB.at(carBrand).frontWheelsYoffset;
	frontWheelsXoffset = carDB.at(carBrand).frontWheelsXoffset;
	backWheelsXoffset = carDB.at(carBrand).backWheelsXoffset;


}

void Car::straightenSteeringWheelAngle()
{
	if (v != 0)
	{
		if (steeringWheelAngle > 0.05f)
			steeringWheelAngle -= 0.05f * v / 100;
		else if (steeringWheelAngle < -0.05f)
			steeringWheelAngle += 0.05f * v / 100;
		else
			steeringWheelAngle = 0.0f;
	}
}

void Car::calculateMovement()
{
	float resultantForceX = 0.0f;
	float resultantForceY = 0.0f;

	for (auto& force : forces)
	{
		resultantForceX += force.value * cos(force.direction);
		resultantForceY += force.value * sin(force.direction);
	}

	float resultantAccelerationX = resultantForceX / mass;
	float resultantAccelerationY = resultantForceY / mass;

	vLoc.x += resultantAccelerationX / FPS;
	vLoc.y += resultantAccelerationY / FPS;

	vx = vLoc.x * cos(rz) - 0.0*vLoc.y * sin(rz);
	vy = vLoc.x * sin(rz) + 0.0*vLoc.y * cos(rz);
	
	position.x += vx / FPS;
	position.y += vy / FPS;
	
	v = sqrt(vx * vx + vy * vy);


	resultantTorque = 0.0f;
	
	auto& force = forces[1];

	resultantTorque += force.center.x * (sin(force.direction) * force.value) - force.center.y * (cos(force.direction) * force.value);


	auto angleToRotate = resultantTorque / FPS / (4500 + mass * backWheelsXoffset * backWheelsXoffset);
	rz += angleToRotate;

	Point newCenter(position.x, position.y);
	Point::rotate(newCenter, getCarPointInGlobalSystem({ backWheelsXoffset, 0 }), angleToRotate);

	position.x = newCenter.x;
	position.y = newCenter.y;
}

void Car::setLastWheelPosition()
{
	lastWheelsPosition[0] = getCarPointInGlobalSystem({ frontWheelsXoffset, frontWheelsYoffset });
	lastWheelsPosition[1] = getCarPointInGlobalSystem({ backWheelsXoffset, frontWheelsYoffset });
	lastWheelsPosition[2] = getCarPointInGlobalSystem({ backWheelsXoffset, -frontWheelsYoffset });
	lastWheelsPosition[3] = getCarPointInGlobalSystem({ frontWheelsXoffset, -frontWheelsYoffset });
}

void Car::calculateCarDrift()
{
	wheelsTrace.push_back(lastWheelsPosition[0]);
	wheelsTrace.push_back(lastWheelsPosition[1]);
	wheelsTrace.push_back(lastWheelsPosition[2]);
	wheelsTrace.push_back(lastWheelsPosition[3]);

	if (wheelsTrace.size() > 1000)
	{
		wheelsTrace.pop_front();
		wheelsTrace.pop_front();
		wheelsTrace.pop_front();
		wheelsTrace.pop_front();
	}

	setLastWheelPosition();
}

void Car::playEngineSound()
{

	float speed = (engine.getRPM() / 3000) + 0.6;

	speed = std::max(speed, 1.0f);

	float volume = sqrt(1 / (std::max(1.0, position.distance2D(*globalCameraCenter)) ));

	vector2D lookingLine(*globalCameraCenter, *globalCameraLookAt);
	vector2D objectLine(*globalCameraCenter, position);

	float angle = vector2D::directedAngle(lookingLine, objectLine);

	float finalPan = 0.0f;

	if (angle >= 0.0f && angle < PI / 2)
		finalPan = -angle / (PI / 2);
	else if (angle >= PI / 2 && angle < PI)
		finalPan = -(PI - angle) / (PI / 2);
	else if (angle >= PI && angle < 3 / 2 * PI)
		finalPan = (angle - PI) / (PI / 2);
	else if (angle >= 3 / 2 * PI && angle < 2 * PI)
		finalPan = (2 * PI - angle) / (PI / 2);

	Game::soundManager.playSound(engineSound, volume, finalPan, speed);
}