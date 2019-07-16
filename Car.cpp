#define NOMINMAX


#include "Car.h"

#include <algorithm>

Car::Car(CarBrand carBrand, float startX, float startY, Point* globalCameraCenter, Point* globalCameraLookAt) : carBrand(carBrand), engine(carBrand), gearBox(carBrand)
{
	importFromObjFile();

	Car::globalCameraCenter = globalCameraCenter;
	Car::globalCameraLookAt = globalCameraLookAt;

	carModelCircles.push_back(Circle{ { 0.0f, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 2, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 2, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 6, width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 6, -width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 6, width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 6, -width / 3.0f, 0.53f }, width / 6 });

	mass = carDB.at(carBrand).mass;
	vMax = carDB.at(carBrand).vMax / 3.6;
	momentOfInertia = 1.0f / 12.0f * mass * (length * length + width * width);

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
	//cameraCenter = Point{ -0.001, 0, 50 };
	cameraLookAt = Point{0, 0, 3};

	setLastWheelPosition();


	engineSound = SoundManager::Instance()->registerSoundInstance(carDB.at(carBrand).engineSound);
	driftSound = SoundManager::Instance()->registerSoundInstance(Sounds::drift);

	pacejkaModel.setCarGeometry(mass, frontWheelsXoffset, frontWheelsYoffset, backWheelsXoffset, rd);

}

void Car::move()
{
	//cameraCenter.y = sin(steeringWheelAngle) * 1.0;

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
	else if (engine.nextGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getNextTransmission(), rd, v.length()))
		gearBox.gearUp();
	else if (engine.previousGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getPreviousTransmission(), rd, v.length()))
		gearBox.gearDown();

	//------------------------

	engine.setRPM(Engine::calculateRMP(gearBox.getCurrentTransmission() * gearBox.getMainTransmission(), rd, v.length()));






	/*float drivingForce = 0.0f;
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

	forces = globalForces;*/

	forces = pacejkaModel.calculateForces(tryAccelerate, trySlow, getGlobalVector(v), angularVelocity, steeringWheelAngle, rz);
	
	calculateCollisions();
	calculateNetForces();
	calculateMovement();

	if (!turning)
		straightenSteeringWheelAngle();
	else
		turning = false;
	calculateCarDrift();

	playEngineSound();
	playDriftSound(pacejkaModel.carDrifting);

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
	steeringWheelAngle -= 0.1;

	/*if (steeringWheelAngle > -0.1 && steeringWheelAngle < 0.1)
		steeringWheelAngle -= 0.016;
	else if (steeringWheelAngle > 0)
		steeringWheelAngle /= 1.1f;
	else
		steeringWheelAngle *= 1.1f;*/

	if (steeringWheelAngle < -PI / 4)
		steeringWheelAngle = -PI / 4;

	turning = true;
}

void Car::turnLeft()
{
	//rz += 0.005;
	steeringWheelAngle += 0.1;

	/*if (steeringWheelAngle > -0.1 && steeringWheelAngle < 0.1)
		steeringWheelAngle += 0.016;
	else if (steeringWheelAngle > 0)
		steeringWheelAngle *= 1.1f;
	else
		steeringWheelAngle /= 1.1f;*/

	if (steeringWheelAngle > PI / 4)
		steeringWheelAngle = PI / 4;

	turning = true;
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

		bool prin3DtModel = false;
		if(prin3DtModel)
		{
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

			for (auto currentWheel :
				std::vector<std::pair<Wheel*, float>>
			{ {&backWheels, pacejkaModel.allWheels[rearLeftWheel].angularVelocity },
			  {&leftWheel,  pacejkaModel.allWheels[frontLeftWheel].angularVelocity },
			  {&rightWheel,  pacejkaModel.allWheels[frontRightWheel].angularVelocity } }
				)
			{
				currentWheel.first->setCarPosition(position.x, position.y, position.z);
				currentWheel.first->setCarAngle(rz);
			
				currentWheel.first->rotate(currentWheel.second);
				currentWheel.first->display();
			}
		}
		else
		{


			glBegin(GL_POLYGON);

			cretateGlobalVertex({ length / 2,  width / 2, 0.5 });
			cretateGlobalVertex({ length / 2, -width / 2, 0.5 });
			cretateGlobalVertex({ -length / 2, -width / 2, 0.5 });
			cretateGlobalVertex({ -length / 2,  width / 2, 0.5 });

			glEnd();

			glColor3f(1.0f, 0.5f, 0.0f);

			for (auto& carModelCircle : carModelCircles)
			{
				Point globalCollisionCircleCenter;
				globalCollisionCircleCenter.x = position.x + carModelCircle.center.x;
				globalCollisionCircleCenter.y = position.y + carModelCircle.center.y;
				globalCollisionCircleCenter.z = carModelCircle.center.z;
				Point::rotate(globalCollisionCircleCenter, position, rz);
				myglDrawFillCircle({ globalCollisionCircleCenter }, carModelCircle.r);
			}

			//glDisable(GL_BLEND);
			//glDisable(GL_TEXTURE_2D);

			//center point
			glPointSize(10.0);
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex3f(position.x, position.y, 0.52);
			glEnd();
			glPointSize(1.0);


			for (auto& force : forces)
			{
				glPointSize(10.0);
				glColor3f(0.0f, 0.0f, 1.0f);
				glBegin(GL_POINTS);
				cretateGlobalVertex(force.center);
				glEnd();
				glPointSize(1.0);

				glLineWidth(5);
				glBegin(GL_LINES);
				cretateGlobalVertex(force.center);
				cretateGlobalVertex({ force.center.x + force.vector.x / 1000, force.center.y + force.vector.y / 1000, force.center.z });
				glEnd();
				glLineWidth(1);
			}

			glLineWidth(5);
			glColor3f(1.0f, 0.5f, 0.0f);
			glBegin(GL_LINES);
			cretateGlobalVertex({ 0, 0, 0.53 });
			cretateGlobalVertex({ v.x * 10, v.y * 10, 0.53 });
			glEnd();
			glLineWidth(1);
		}

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

	glColor3f(1.0f, 0.0f, 1.0f);
	glPointSize(10);
	glBegin(GL_POINTS);

	glVertex3f(obstacle.center.x, obstacle.center.y, obstacle.center.z);

	glEnd();
	glPointSize(1);

	glColor3f(1.0f, 0.0f, 0.5f);
	myglDrawFillCircle(obstacle.center, obstacle.r);
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

	float maxX = -1000.0f;
	float maxY = -1000.0f;
	float minX =  1000.0f;
	float minY =  1000.0f;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			if (point.x > maxX)
				maxX = point.x;
			if (point.y > maxY)
				maxY = point.y;
			if (point.x < minX)
				minX = point.x;
			if (point.y < minY)
				minY = point.y;
		}
	}

	length = maxX - minX;
	width = maxY - minY;
}

void Car::straightenSteeringWheelAngle()
{
	if (v.length() != 0)
	{
		if (steeringWheelAngle > 0.15f)
			steeringWheelAngle -= 0.15f * v.length() / 50;
		else if (steeringWheelAngle < -0.15f)
			steeringWheelAngle += 0.15f * v.length() / 50;
		else
			steeringWheelAngle = 0.0f;
	}
}

void Car::calculateMovement()
{
	
	if (v.x * (v.x + netForce.vector.x / mass / FPS) < 0)
		v.x = 0.0f;
	else
		v.x += netForce.vector.x / mass / FPS;
	
	if (v.y * (v.y + netForce.vector.y / mass / FPS) < 0)
		v.y = 0.0f;
	else
		v.y += netForce.vector.y / mass / FPS;

	if (abs(v.x) < 0.01f)
		v.x = 0;
	if (abs(v.y) < 0.1f)
		v.y = 0;

	velocity = v.length();

	sin_rz = sin(rz);
	cos_rz = cos(rz);

	
	//position.x += v.x / FPS;
	//position.y += v.y / FPS;
	position.x += (v.x * cos_rz - v.y * sin_rz) / FPS;
	position.y += (v.x * sin_rz + v.y * cos_rz) / FPS;


	if (angularVelocity * (angularVelocity + netTorque / momentOfInertia / FPS) < 0)
		angularVelocity = 0.0f;
	else
		angularVelocity += netTorque / momentOfInertia / FPS;
	
	if (abs(angularVelocity) < 0.1f)
		angularVelocity = 0;


	rz += angularVelocity / FPS;


	obstacle.center.x += obstacleV.x / FPS;
	obstacle.center.y += obstacleV.y / FPS;
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

	SoundManager::Instance()->playSound(engineSound, volume, finalPan, speed);
}

void Car::playDriftSound(bool carDrifting)
{
	float volume = carDrifting * sqrt(1 / (std::max(1.0, position.distance2D(*globalCameraCenter))));

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

	SoundManager::Instance()->playSound(driftSound, volume, finalPan, 1.0f);
}

void Car::calculateNetForces()
{
	Force newNetForce;
	float newNetTorque = 0.0f;

	for (auto& force : forces)
	{
		newNetForce.vector += force.vector;
		newNetForce.center.z = 0.52f;

		vector2D r({0, 0}, force.center);
		newNetTorque += vector2D::crossProduct(r, force.vector);
	}

	netForce = newNetForce;
	netTorque = newNetTorque;
}

void Car::calculateCollisions()
{
	std::vector<std::pair<Circle*, Circle*>> collidingObjects;

	for (auto& carModelCircle : carModelCircles)
	{
		Point globalCollisionCircleCenter;
		globalCollisionCircleCenter.x = position.x + carModelCircle.center.x;
		globalCollisionCircleCenter.y = position.y + carModelCircle.center.y;
		globalCollisionCircleCenter.z = carModelCircle.center.z;
		Point::rotate(globalCollisionCircleCenter, position, rz);

		Circle globalCollisionCircle{ globalCollisionCircleCenter, carModelCircle.r };

		if (globalCollisionCircle.isColliding(obstacle))
		{
			collidingObjects.push_back({ &obstacle, &carModelCircle });

			Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 75, 50, "Collision!", &(Screen2D::Instance()->roboto_modo_regular));

			float fDistance = sqrtf((globalCollisionCircle.center.x - obstacle.center.x)*(globalCollisionCircle.center.x - obstacle.center.x) + (globalCollisionCircle.center.y - obstacle.center.y)*(globalCollisionCircle.center.y - obstacle.center.y));

			// Calculate displacement required
			float fOverlap = 0.5f * (fDistance - globalCollisionCircle.r - obstacle.r);

			// Displace Current Ball away from collision
			position.x -= 2.0f * fOverlap * (globalCollisionCircle.center.x - obstacle.center.x) / fDistance;
			position.y -= 2.0f * fOverlap * (globalCollisionCircle.center.y - obstacle.center.y) / fDistance;

			// Displace Target Ball away from collision
			//obstacle.center.x += fOverlap * (globalCollisionCircle.center.x - obstacle.center.x) / fDistance;
			//obstacle.center.y += fOverlap * (globalCollisionCircle.center.y - obstacle.center.y) / fDistance;

			break;
		}
	}

	if (!collidingObjects.empty())
	{
		for (auto& collidingObject : collidingObjects)
		{
			Point carModelCircleGlobalCenter;
			carModelCircleGlobalCenter.x = position.x + collidingObject.second->center.x;
			carModelCircleGlobalCenter.y = position.y + collidingObject.second->center.y;
			carModelCircleGlobalCenter.z = collidingObject.second->center.z;
			Point::rotate(carModelCircleGlobalCenter, position, rz);

			Circle carModelGlobalCircle{ carModelCircleGlobalCenter, collidingObject.second->r };

			auto& collisionCircle = carModelGlobalCircle;
			// Distance between balls
			float fDistance = sqrtf((collisionCircle.center.x - collidingObject.first->center.x)*(collisionCircle.center.x - collidingObject.first->center.x) + (collisionCircle.center.y - collidingObject.first->center.y)*(collisionCircle.center.y - collidingObject.first->center.y));
			
			// Normal
			float nx = (collidingObject.first->center.x - collisionCircle.center.x) / fDistance;
			float ny = (collidingObject.first->center.y - collisionCircle.center.y) / fDistance;
			
			// Tangent
			//float tx = -ny;
			//float ty = nx;
			//
			vector2D vCarGlobal = getGlobalVector(v);
			//
			//// Dot Product Tangent
			//float dpTan1 = vCarGlobal.x * tx + vCarGlobal.y * ty;
			//float dpTan2 = obstacleV.x * tx + obstacleV.y * ty;
			//
			//// Dot Product Normal
			//float dpNorm1 = vCarGlobal.x * nx + vCarGlobal.y * ny;
			//float dpNorm2 = obstacleV.x * nx + obstacleV.y * ny;
			//
			//// Conservation of momentum in 1D
			//float m1 = (dpNorm1 * (mass - obstacleMass) + 2.0f * obstacleMass * dpNorm2) / (mass + obstacleMass);
			//float m2 = (dpNorm2 * (obstacleMass - mass) + 2.0f * mass * dpNorm1) / (mass + obstacleMass);
			//
			//// Update ball velocities
			////v.x = tx * dpTan1 + nx * m1;
			////v.y = ty * dpTan1 + ny * m1;
			//obstacleV.x = tx * dpTan2 + nx * m2;
			//obstacleV.y = ty * dpTan2 + ny * m2;

			//vector2D vCarGlobal;
			//vCarGlobal.x = v.x * cos(-rz) - v.y * sin(-rz);
			//vCarGlobal.y = v.x * sin(-rz) + v.y * cos(-rz);
			//

			vector2D vCircleGlobal(vCarGlobal.x, vCarGlobal.y);

			float kx = (vCircleGlobal.x - obstacleV.x);
			float ky = (vCircleGlobal.y - obstacleV.y);
			float p = 2.0 * (nx * kx + ny * ky) / (mass + obstacleMass);

			vector2D modelCircleGlobalVelocity{ -p * obstacleMass * nx, -p * obstacleMass * ny };

			vCarGlobal.x += modelCircleGlobalVelocity.x;
			vCarGlobal.y += modelCircleGlobalVelocity.y;
			obstacleV.x = obstacleV.x + p * mass * nx;
			obstacleV.y = obstacleV.y + p * mass * ny;

			//v.x = vCarGlobal.x * cos(-rz) - vCarGlobal.y * sin(-rz);
			//v.y = vCarGlobal.x * sin(-rz) + vCarGlobal.y * cos(-rz);
			v = getLocalVector(vCarGlobal);
			//v = getLocalVector(vCarGlobal);

			if(abs(collisionCircle.center.y - position.y) > 0.1)
				angularVelocity -= modelCircleGlobalVelocity.x / (collisionCircle.center.y - position.y);
			if (abs(collisionCircle.center.x - position.x) > 0.1)
				angularVelocity += modelCircleGlobalVelocity.y / (collisionCircle.center.x - position.x);
		}
		collidingObjects.clear();
	}
}