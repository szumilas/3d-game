#define NOMINMAX

#include "Car.h"
#include "MapContainer.h"
#include "CameraManager.h"
#include "GameClock.h"

#include <algorithm>

Car::Car(CarBrand carBrand, float startX, float startY, bool humanCar) : carBrand(carBrand), engine(carBrand), gearBox(carBrand), humanCar(humanCar)
{
	importFromObjFile();

	Car::globalCameraCenter = &CameraManager::Instance()->center;
	Car::globalCameraLookAt = &CameraManager::Instance()->lookAt;

	carModelCircles.push_back(Circle{ { 0.0f, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 2, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 2, 0.0f, 0.53f }, width / 2 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 6, width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { length / 2 - width / 6, -width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 6, width / 3.0f, 0.53f }, width / 6 });
	carModelCircles.push_back(Circle{ { -length / 2 + width / 6, -width / 3.0f, 0.53f }, width / 6 });

	mass = carDB.at(carBrand).mass;
	vMax = carDB.at(carBrand).vMax / 3.6;
	momentOfInertia = 1.50f / 12.0f * mass * (length * length + width * width);

	//position.x = startX -140;
	//position.y = startY -347;
	position.x = startX + 355;
	position.y = startY - 130;
	position.z = 0.01;

	rz = 3.14 / 4 * 0 + 3.6;

	steeringWheelAngle = 0;

	wheels.resize(4);

	for (auto& wheel : wheels)
	{
		wheel.loadModel();
	}

	setBackCamera();

	setLastWheelPosition();


	engineSound = SoundManager::Instance()->registerSoundInstance(carDB.at(carBrand).engineSound);
	driftSound = SoundManager::Instance()->registerSoundInstance(Sounds::drift);

	pacejkaModel.setHumanCar(humanCar);
	pacejkaModel.setCarGeometry(mass, frontWheelsXoffset, frontWheelsYoffset, backWheelsXoffset, rd, 100.0f / 3.6f / carDB.at(carBrand).acceleration_0_100, carDB.at(carBrand).vMax / 3.6);

	AIcurrentPoint = 0;
}

Car::~Car()
{
	SoundManager::Instance()->playSound(engineSound, 0);
	SoundManager::Instance()->playSound(driftSound, 0);
}

void Car::findAproximateAIPathPosition()
{
	if (MapContainer::Instance()->AIPoints.size() > 1)
	{
		Point& p1 = MapContainer::Instance()->AIPoints[AIcurrentPoint].center;
		Point& p2 = MapContainer::Instance()->AIPoints[(AIcurrentPoint + 1) % MapContainer::Instance()->AIPoints.size()].center;

		auto u = vector2D::dotProduct(vector2D(p1, position), vector2D(p1, p2)) / vector2D::dotProduct(vector2D(p2, p1), vector2D(p2, p1));

		if (u > 0)
			AIcurrentPoint = (AIcurrentPoint + 1) % MapContainer::Instance()->AIPoints.size();
	}
}

void Car::update()
{
	move();
	updateGhostCarAbility();
}

void Car::move()
{
	if (humanCar == 1)
		findAproximateAIPathPosition();
	else
		AImove();

	drivingDir = drivingDirection();
	//cameraCenter.y = sin(steeringWheelAngle) * 1.0;

	//Automatic Transmission
	//------------------------

	if (drivingDir == 1 || v.length() < 0.2)
	{
		if (tryAccelerate && gearBox.getCurrentGear() == 0)
			gearBox.gearUp();
		else if (engine.nextGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getNextTransmission(), rd, v.length()))
			gearBox.gearUp();
		else if (engine.previousGearDrivingForceBigger(gearBox.getMainTransmission(), gearBox.getCurrentTransmission(), gearBox.getPreviousTransmission(), rd, v.length()))
			gearBox.gearDown();
	}
	else
	{
		gearBox.setReverseGear();
	}

	//------------------------

	engine.setRPM(Engine::calculateRMP(gearBox.getCurrentTransmission() * gearBox.getMainTransmission(), rd, v.length()));

	if (!MapContainer::Instance()->raceTimer.beforeRace)
	{
		forces = pacejkaModel.calculateForces(drivingDir, tryAccelerate, trySlow, tryBreak, getGlobalVector(v), v, acceleration, angularVelocity, steeringWheelAngle, rz);
	}
	
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

	if (tryAccelerate == false && trySlow == false && v.length() < 0.5)
		stop();

	//if (v.x <= -2)
	//	v.x = 50;

	tryAccelerate = false;
	trySlow = false;
	tryBreak = false;

	if (humanCar)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::BLACK, -100, 60, "wheel velocity: " + std::to_string(angularVelocity), &(Screen2D::Instance()->roboto_modo_regular));
	}
	//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 80, "x: " + std::to_string(position.x) + "   y: " + std::to_string(position.y), &(Screen2D::Instance()->roboto_modo_regular));
	//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, -50, 80, "a.x: " + std::to_string(acceleration.x) + "   a.y: " + std::to_string(acceleration.y), &(Screen2D::Instance()->roboto_modo_regular));
}

void Car::accelerate()
{
	tryAccelerate = true;
}

void Car::slow()
{
	trySlow = true;
}

void Car::breakPressed()
{
	tryBreak = true;
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
	static int ghostCarCounter = 0;
	ghostCarCounter++;

	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

		sin_rz = sin(rz);
		cos_rz = cos(rz);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		bool prin3DtModel = true;
		if(prin3DtModel)
		{
			if (!ghostCar || (ghostCarCounter / 20) % 2 == 0)
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


		for (auto& point : temporary)
		{

			glColor3f(0.0f, 1.0f, 0.0f);
			myglDrawFillCircle( point, 0.1 );
		}
		temporary.clear();

	}

	//glColor3f(1.0f, 0.0f, 1.0f);
	//glPointSize(10);
	//glBegin(GL_POINTS);
	//
	//glVertex3f(obstacle.center.x, obstacle.center.y, obstacle.center.z);
	//
	//glEnd();
	//glPointSize(1);

	//glColor3f(1.0f, 0.0f, 0.5f);
	//myglDrawFillCircle(obstacle.center, obstacle.r);
}

void Car::importFromObjFile()
{
	auto materials = Object3D::importMaterials((carDB.at(carBrand).objFilePath + "materials.mtl").c_str());

	float scaleRatio = 1.0;
	auto originalCarBoundaries = Object3D::importFromObjFile((carDB.at(carBrand).objFilePath + "body.obj").c_str(), carDB.at(carBrand).textureName, scaleRatio, carDB.at(carBrand).totalLength, &materials);
	auto originalBackWheelBoundaries = backWheels.importFromObjFile((carDB.at(carBrand).objFilePath + "back_wheels.obj").c_str(), carDB.at(carBrand).textureName, scaleRatio);
	leftWheel.importFromObjFile((carDB.at(carBrand).objFilePath + "left_wheel.obj").c_str(), carDB.at(carBrand).textureName, scaleRatio);
	auto originalFrontWheelBoundaries = rightWheel.importFromObjFile((carDB.at(carBrand).objFilePath + "right_wheel.obj").c_str(), carDB.at(carBrand).textureName, scaleRatio);
	
	frontWheelsXoffset = scaleRatio * ((originalCarBoundaries.second.x + originalCarBoundaries.first.x) / 2 - (originalFrontWheelBoundaries.second.x + originalFrontWheelBoundaries.first.x) / 2);
	frontWheelsYoffset = scaleRatio * ((originalCarBoundaries.second.y + originalCarBoundaries.first.y) / 2 - (originalFrontWheelBoundaries.second.y + originalFrontWheelBoundaries.first.y) / 2);
	backWheelsXoffset = scaleRatio * ((originalCarBoundaries.second.x + originalCarBoundaries.first.x) / 2 - (originalBackWheelBoundaries.second.x + originalBackWheelBoundaries.first.x) / 2);
	
	backWheels.calculateGeometry(backWheelsXoffset);
	leftWheel.calculateGeometry(frontWheelsXoffset, frontWheelsYoffset);
	rightWheel.calculateGeometry(frontWheelsXoffset, -frontWheelsYoffset);


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
	{
		v.x = 0.0f;
		acceleration.x = 0.0f;
	}
	else
	{
		acceleration.x = netForce.vector.x / mass;
		v.x += acceleration.x / FPS;
	}
	
	if (v.y * (v.y + netForce.vector.y / mass / FPS) < 0)
	{
		v.y = 0.0f;
		acceleration.y = 0.0f;
	}
	else
	{
		acceleration.y = netForce.vector.y / mass;
		v.y += acceleration.y / FPS;
	}

	if (abs(v.x) < 0.01f)
		v.x = 0;
	if (abs(v.y) < 0.1f)
		v.y = 0;

	velocity = v.length();

	sin_rz = sin(rz);
	cos_rz = cos(rz);
	
	position.x += (v.x * cos_rz - v.y * sin_rz) / FPS;
	position.y += (v.x * sin_rz + v.y * cos_rz) / FPS;

	//acceleration = getLocalVector(acceleration);


	if (angularVelocity * (angularVelocity + netTorque / momentOfInertia / FPS) < 0)
		angularVelocity = 0.0f;
	else
		angularVelocity += netTorque / momentOfInertia / FPS;
	
	if (abs(angularVelocity) < 0.1f)
		angularVelocity = 0;


	rz += angularVelocity / FPS;


	//obstacle.center.x += obstacleV.x / FPS;
	//obstacle.center.y += obstacleV.y / FPS;
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
	float volume = 0.8 * carDrifting * sqrt(1 / (std::max(1.0, position.distance2D(*globalCameraCenter))));

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

void Car::playCrashSound(const vector2D& modelCircleGlobalVelocity)
{
	float volume = std::min(modelCircleGlobalVelocity.length() / 5, 1.0) * sqrt(1 / (std::max(1.0, position.distance2D(*globalCameraCenter))));

	Sounds crash;
	if (static_cast<int>(position.x * 100) % 3 == 0)
		crash = Sounds::crash_01;
	else if (static_cast<int>(position.x * 100) % 3 == 1)
		crash = Sounds::crash_02;
	else
		crash = Sounds::crash_03;

	SoundManager::Instance()->playSample(crash, volume, 0, 1);
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
	std::vector<std::tuple<Circle*, Circle*, float>> collidingObjects;
	std::vector<std::tuple<Circle, float, vector2D>> obstaclesWithPossibleCollision;
	static float infiniteMass = 999999999.0;
	vector2D obstacleV;

	for (auto& enemyCar : MapContainer::Instance()->cars)
	{
		if (enemyCar.carBrand != carBrand && !enemyCar.ghostCar && !ghostCar)
		{
			for (auto& enemyCarModelCircle : enemyCar.carModelCircles)
			{
				Circle newObstacle;

				Point globalCollisionCircleCenter;
				globalCollisionCircleCenter.x = enemyCar.position.x + enemyCarModelCircle.center.x;
				globalCollisionCircleCenter.y = enemyCar.position.y + enemyCarModelCircle.center.y;
				globalCollisionCircleCenter.z = enemyCarModelCircle.center.z;
				Point::rotate(globalCollisionCircleCenter, enemyCar.position, enemyCar.rz);

				newObstacle.center = globalCollisionCircleCenter;
				newObstacle.r = enemyCarModelCircle.r;
				if (newObstacle.center.distance2D(position) > 10.0)
					continue;

				vector2D enemyCarGlobalVelocity = { enemyCar.v.x * cos(enemyCar.rz) - enemyCar.v.y * sin(enemyCar.rz), enemyCar.v.x * sin(enemyCar.rz) + enemyCar.v.y * cos(enemyCar.rz) };

				obstaclesWithPossibleCollision.push_back({ newObstacle, enemyCar.mass, enemyCarGlobalVelocity });
			}
		}
	}

	for (auto& obstacles : MapContainer::Instance()->getCollidableObjectsInPosition(position))
	{
		for (auto& currentObstacle : *obstacles)
		{
			if (currentObstacle->get()->collidable == Collidable::point)
			{
				Circle newObstacle;
				newObstacle.center = currentObstacle->get()->position;
				newObstacle.r = 0.5f;
				if (newObstacle.center.distance2D(position) > 5.0)
					continue;
				obstaclesWithPossibleCollision.push_back({ newObstacle, infiniteMass, {0, 0} });
			}
			else if (currentObstacle->get()->collidable == Collidable::polygon)
			{
				for (auto& carModelCircle : carModelCircles)
				{
					for (int i = 0; i < currentObstacle->get()->points.size() - 1; i++)
					{
						auto& p1 = currentObstacle->get()->points[i];
						auto& p2 = currentObstacle->get()->points[i + 1];
						Point globalCollisionCircleCenter;
						globalCollisionCircleCenter.x = position.x + carModelCircle.center.x;
						globalCollisionCircleCenter.y = position.y + carModelCircle.center.y;
						globalCollisionCircleCenter.z = carModelCircle.center.z;
						Point::rotate(globalCollisionCircleCenter, position, rz);

						auto u = vector2D::dotProduct(vector2D(p1, globalCollisionCircleCenter), vector2D(p1, p2)) / vector2D::dotProduct(vector2D(p2, p1), vector2D(p2, p1));

						Point wallPoint(p1);
						wallPoint.x += u * (p2.x - p1.x);
						wallPoint.y += u * (p2.y - p1.y);

						if (u > 0 && u < 1.0 && globalCollisionCircleCenter.distance2D(wallPoint) < 5.0)
						{
							temporary.push_back(wallPoint);
							obstaclesWithPossibleCollision.push_back({ { wallPoint, 0.2f }, infiniteMass,{ 0, 0 } });
						}
					}
				}
			}
		}
	}

	for (auto& carModelCircle : carModelCircles)
	{
		Point globalCollisionCircleCenter;
		globalCollisionCircleCenter.x = position.x + carModelCircle.center.x;
		globalCollisionCircleCenter.y = position.y + carModelCircle.center.y;
		globalCollisionCircleCenter.z = carModelCircle.center.z;
		Point::rotate(globalCollisionCircleCenter, position, rz);

		Circle globalCollisionCircle{ globalCollisionCircleCenter, carModelCircle.r };

		for (auto& obstacle : obstaclesWithPossibleCollision)
		{
			if (globalCollisionCircle.isColliding(std::get<0>(obstacle)))
			{
				obstacleV = std::get<2>(obstacle);

				collidingObjects.push_back({ &std::get<0>(obstacle), &carModelCircle, std::get<1>(obstacle) });

				Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 75, 50, "Collision!", &(Screen2D::Instance()->roboto_modo_regular));

				float fDistance = sqrtf((globalCollisionCircle.center.x - std::get<0>(obstacle).center.x)*(globalCollisionCircle.center.x - std::get<0>(obstacle).center.x) + (globalCollisionCircle.center.y - std::get<0>(obstacle).center.y)*(globalCollisionCircle.center.y - std::get<0>(obstacle).center.y));

				// Calculate displacement required
				float fOverlap = 0.5f * (fDistance - globalCollisionCircle.r - std::get<0>(obstacle).r);

				// Displace Current Ball away from collision
				position.x -= 2.0f * fOverlap * (globalCollisionCircle.center.x - std::get<0>(obstacle).center.x) / fDistance;
				position.y -= 2.0f * fOverlap * (globalCollisionCircle.center.y - std::get<0>(obstacle).center.y) / fDistance;

				break;
			}
		}

		if (!collidingObjects.empty())
			break;
	}

	if (!collidingObjects.empty())
	{
		for (auto& collidingObject : collidingObjects)
		{
			Point carModelCircleGlobalCenter;
			carModelCircleGlobalCenter.x = position.x + std::get<1>(collidingObject)->center.x;
			carModelCircleGlobalCenter.y = position.y + std::get<1>(collidingObject)->center.y;
			carModelCircleGlobalCenter.z = std::get<1>(collidingObject)->center.z;
			Point::rotate(carModelCircleGlobalCenter, position, rz);

			Circle carModelGlobalCircle{ carModelCircleGlobalCenter, std::get<1>(collidingObject)->r };

			auto& collisionCircle = carModelGlobalCircle;
			// Distance between balls
			float fDistance = sqrtf((collisionCircle.center.x - std::get<0>(collidingObject)->center.x)*(collisionCircle.center.x - std::get<0>(collidingObject)->center.x) + (collisionCircle.center.y - std::get<0>(collidingObject)->center.y)*(collisionCircle.center.y - std::get<0>(collidingObject)->center.y));
			
			// Normal
			float nx = (std::get<0>(collidingObject)->center.x - collisionCircle.center.x) / fDistance;
			float ny = (std::get<0>(collidingObject)->center.y - collisionCircle.center.y) / fDistance;

			vector2D vCarGlobal = getGlobalVector(v);

			vector2D vCircleGlobal(vCarGlobal.x, vCarGlobal.y);

			float kx = (vCircleGlobal.x - obstacleV.x);
			float ky = (vCircleGlobal.y - obstacleV.y);
			float p = 2.0 * (nx * kx + ny * ky) / (mass + std::get<2>(collidingObject));

			vector2D modelCircleGlobalVelocity{ -p * std::get<2>(collidingObject) * nx, -p * std::get<2>(collidingObject) * ny };

			auto energyBeforeCollision = pow(vCarGlobal.length(), 2);
			vCarGlobal.x += modelCircleGlobalVelocity.x;
			vCarGlobal.y += modelCircleGlobalVelocity.y;
			auto energyAfterCollision = pow(vCarGlobal.length(), 2);

			static double maxEneryRatio = 0.3;

			if (obstacleV.length() < 0.1 && abs(energyBeforeCollision) > 0.1 && energyAfterCollision / energyBeforeCollision > maxEneryRatio)
			{
				vCarGlobal *= maxEneryRatio;
			}

			playCrashSound(modelCircleGlobalVelocity);
			
			obstacleV.x = obstacleV.x + p * mass * nx;
			obstacleV.y = obstacleV.y + p * mass * ny;

			v = getLocalVector(vCarGlobal);

			if(abs(collisionCircle.center.y - position.y) > 0.2)
				angularVelocity -= modelCircleGlobalVelocity.x / (collisionCircle.center.y - position.y) / 6;
			if (abs(collisionCircle.center.x - position.x) > 0.2)
				angularVelocity += modelCircleGlobalVelocity.y / (collisionCircle.center.x - position.x) / 6;

			if (angularVelocity > PI / 2)
				angularVelocity = PI / 2;
			if (angularVelocity < -PI / 2)
				angularVelocity = -PI / 2;
		}
		drivingDir = drivingDirection();
		pacejkaModel.recalculateWheelAngularVelocity(drivingDir * v.length(), true);
		collidingObjects.clear();
	}
}

int Car::drivingDirection()
{
	auto angle = vector2D::angle(vector2D(cos(rz), sin(rz)), getGlobalVector(v));
	if (angle > PI / 2.0)
		return -1;
	else
		return 1;
}

void Car::AImove()
{
	if (MapContainer::Instance()->AIPoints.size() < 3 || MapContainer::Instance()->getAIPathActive() == false)
		return;

	int previousAIPoint = AIcurrentPoint;

	if (MapContainer::Instance()->AIPoints[AIcurrentPoint].center.distance2D(position) < 5.0)
		AIcurrentPoint = MapContainer::Instance()->GetNextPoint(AIcurrentPoint);

	auto possibleCloserPoint = MapContainer::Instance()->GetNextPoint(AIcurrentPoint);
	if (MapContainer::Instance()->AIPoints[AIcurrentPoint].center.distance2D(position) > MapContainer::Instance()->AIPoints[possibleCloserPoint].center.distance2D(position))
	{
		AIcurrentPoint = possibleCloserPoint;
	}

	if (previousAIPoint != AIcurrentPoint)
	{
		AIPathResetCounter();
	}

	Point& AIdirection = MapContainer::Instance()->AIPoints[AIcurrentPoint].center;

	vector2D AIpathAngle(position, AIdirection);
	AIpathAngle.rotate(-rz - steeringWheelAngle);
	auto angleToFollow = vector2D::realAngle(AIpathAngle, vector2D({ 0,0 }, { 1,0 }));
	angleToFollow -= angularVelocity * 10 / FPS;
	//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 80, "angle to follow: " + std::to_string(angleToFollow), &(Screen2D::Instance()->roboto_modo_regular));
	
	if (angleToFollow < PI * 0.02 || angleToFollow > PI + PI * 0.98)
	{
		//direction is correct, do nothing
	}
	else if (angleToFollow < PI)
	{
		turnLeft();
	}
	else if (angleToFollow > PI)
	{
		turnRight();
	}


	float maxFutureSpeedAngleToFollow = 0;

	MapContainer::Instance()->ClearFuturePoints();
	int AIfuturePoint = AIcurrentPoint;
	int next = 0;

	float q = 0;

	for(float time = 4.0f - (MapContainer::Instance()->AIPoints[AIcurrentPoint].center.distance2D(position)) / v.length(); time > 0; time -= MapContainer::Instance()->GetNextPointDistance(AIfuturePoint) / v.length())
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::GREEN, 25, 60 - q * 3, "time: " + std::to_string(time), &(Screen2D::Instance()->roboto_modo_regular));

		MapContainer::Instance()->SetFuturePoints(AIfuturePoint);
		Point& AIdirection = MapContainer::Instance()->AIPoints[AIfuturePoint].center;
		vector2D AIpathAngle(position, AIdirection);
		AIpathAngle.rotate(-rz - steeringWheelAngle);
		auto futureSpeedAngleToFollow = vector2D::realAngle(AIpathAngle, vector2D({ 0,0 }, { 1,0 }));
		futureSpeedAngleToFollow -= angularVelocity * 10 / FPS;

		while (futureSpeedAngleToFollow < 0)
			futureSpeedAngleToFollow += 2 * PI;

		AIfuturePoint = MapContainer::Instance()->GetNextPoint(AIfuturePoint);

		if (futureSpeedAngleToFollow > PI)
			futureSpeedAngleToFollow = 2 * PI - futureSpeedAngleToFollow;

		if (futureSpeedAngleToFollow > maxFutureSpeedAngleToFollow)
			maxFutureSpeedAngleToFollow = futureSpeedAngleToFollow;

		q++;
	}

	static float safeVelocity = 100.0 / 3.6; //[m/s]

	//Screen2D::Instance()->addTestValueToPrint(ColorName::GREEN, 25, 85, std::to_string(maxFutureSpeedAngleToFollow), &(Screen2D::Instance()->roboto_modo_regular));

	if (maxFutureSpeedAngleToFollow > PI * 0.5 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.5)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 1", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if (maxFutureSpeedAngleToFollow > PI * 0.4 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.4 && v.length() > 0.3 * safeVelocity)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 2", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if (maxFutureSpeedAngleToFollow > PI * 0.3 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.3 && v.length() > 0.5 * safeVelocity)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 3", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if (maxFutureSpeedAngleToFollow > PI * 0.2 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.2 && v.length() > 0.7 * safeVelocity)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 4", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if (maxFutureSpeedAngleToFollow > PI * 0.1 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.1 && v.length() > 0.8 * safeVelocity)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 5", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if (maxFutureSpeedAngleToFollow > PI * 0.05 && maxFutureSpeedAngleToFollow < 2 * PI - PI * 0.05 && v.length() > 1.0 * safeVelocity)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "break 6", &(Screen2D::Instance()->roboto_modo_regular));
		breakPressed();
	}
	else if(pacejkaModel.carDrifting)
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "drift", &(Screen2D::Instance()->roboto_modo_regular));
		//breakPressed();
	}
	else
	{
		//Screen2D::Instance()->addTestValueToPrint(ColorName::RED, 25, 90, "accelerate", &(Screen2D::Instance()->roboto_modo_regular));
		accelerate();
	}
}

void Car::mute()
{
	SoundManager::Instance()->playSound(driftSound, 0);
	SoundManager::Instance()->playSound(engineSound, 0);
}

void Car::resetPositionToAIPath()
{
	if (!MapContainer::Instance()->AIPoints.empty() && !ghostCar)
	{
		stop();
		Point& p1 = MapContainer::Instance()->AIPoints[AIcurrentPoint].center;
		Point& p2 = MapContainer::Instance()->AIPoints[(AIcurrentPoint + 1) % MapContainer::Instance()->AIPoints.size()].center;

		position = p1;
		rz = vector2D::directedAngle(vector2D({0, 0}, {1, 0}), vector2D(p1, p2));

		ghostCar = true;
		ghostCarTimer = GameClock::Instance()->getClock();
	}
}

void Car::updateGhostCarAbility()
{
	if (ghostCar)
	{
		if (GameClock::Instance()->getClock() - ghostCarTimer > 3000)
		{
			ghostCar = false;
			ghostCarTimer = 0;
		}
	}
	else
	{
		if (!humanCar)
		{
			if (GameClock::Instance()->getClock() - AIcurrentPointTimer > 6000)
			{
				AIcurrentPointTimer = GameClock::Instance()->getClock();
				resetPositionToAIPath();
			}
		}
	}
}

void Car::AIPathResetCounter()
{
	AIcurrentPointTimer = GameClock::Instance()->getClock();
}