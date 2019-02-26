#include "Car.h"
#include <fstream>
#include <sstream>

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

void Car::importFromObjFile(const char* filePath, TextureManager* textureManager, Textures textureName, float scaleRatio)
{
	std::ifstream file;
	file.open(filePath);

	if (!file)
		throw;

	std::vector<Point> vertices(1);
	std::vector<Point> textureVertices(1);
	std::vector<std::vector<int>> faces;


	std::string line;
	std::string svalue;

	do
	{
		getline(file, line);
		std::stringstream ssline(line);

		ssline >> svalue;

		if (svalue == "v")
		{
			Point newPoint;
			ssline >> newPoint.x;
			ssline >> newPoint.z;
			ssline >> newPoint.y;

			//newPoint.x /= 10;
			//newPoint.y /= 10;
			//newPoint.z /= 10;

			vertices.push_back(newPoint);
		}
		else if (svalue == "vt")
		{
			Point newPoint;
			ssline >> newPoint.x;
			ssline >> newPoint.y;

			textureVertices.push_back(newPoint);
		}
		else if (svalue == "f")
		{
			int v = 0, vt = 0, vn = 0;
			Polygon newPolygon;

			std::stringstream ssfulldata(line);
			getline(ssfulldata, svalue, ' ');

			while (getline(ssfulldata, svalue, ' '))
			{
				std::stringstream data(svalue);
				getline(data, svalue, '/');
				v = stoi(svalue);

				getline(data, svalue, '/');
				vt = stoi(svalue);

				getline(data, svalue, '/');
				vn = stoi(svalue);

				newPolygon.points.push_back(vertices[v]);
				newPolygon.texturePoints.push_back(textureVertices[vt]);
			}

			newPolygon.noOfPoints = newPolygon.points.size();
			newPolygon.color = Color::white();
			newPolygon.idTexture = textureManager->textures[static_cast<int>(textureName)].idTexture;
			polygons.push_back(newPolygon);

		}
		else if (svalue == "usemtl")
		{
			//vertices.resize(1);
			//textureVertices.resize(1);
		}
	} while (line != "#EOF" && line != "# End of File");

	float maxX = 0.0f;
	float maxY = 0.0f;
	float minX = 100.0f;
	float minY = 100.0f;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			if (point.x > maxX)
			{
				maxX = point.x;
			}
			if (point.y > maxY)
			{
				maxY = point.y;
			}
			if (point.x < minX)
			{
				minX = point.x;
			}
			if (point.y < minY)
			{
				minY = point.y;
			}
		}

	}

	auto deltaX = maxX - minX;
	auto deltaY = maxY - minY;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			point.x = point.x - minX - 0.5 * deltaX;
			point.y = point.y - minY - 0.5 * deltaY;
			
			point.x *= -scaleRatio;
			point.y *= scaleRatio;
			point.z *= scaleRatio;
		}
	}

	file.close();
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