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

}

void Car::importFromObjFile(const char* filePath, TextureManager* textureManager)
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
			newPolygon.idTexture = textureManager->textures[static_cast<int>(Textures::octavia)].idTexture;
			polygons.push_back(newPolygon);

		}
		else if (svalue == "usemtl")
		{
			//vertices.resize(1);
			//textureVertices.resize(1);
		}
	} while (line != "#EOF");

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
			
			point.x *= -0.165;
			point.y *= 0.165;
			point.z *= 0.165;
		}
	}

	file.close();
}

void Car::loadModel()
{
	mesh.push_back({ 2.063f, 0.82f, 0.287f });
	mesh.push_back({ 1.623f, 0.82f, 0.255f });
	mesh.push_back({ 1.607f, 0.82f, 0.403f });
	mesh.push_back({ 1.503f, 0.82f, 0.551f });
	mesh.push_back({ 1.359f, 0.82f, 0.619f });
	mesh.push_back({ 1.159f, 0.82f, 0.607f });
	mesh.push_back({ 1.015f, 0.82f, 0.499f });
	mesh.push_back({ 0.951f, 0.82f, 0.279f });
	mesh.push_back({ -0.189f, 0.82f, 0.291f });
	mesh.push_back({ -0.881f, 0.82f, 0.243f });
	mesh.push_back({ -0.893f, 0.82f, 0.407f });
	mesh.push_back({ -1.005f, 0.82f, 0.551f });
	mesh.push_back({ -1.209f, 0.82f, 0.615f });
	mesh.push_back({ -1.385f, 0.82f, 0.567f });
	mesh.push_back({ -1.493f, 0.82f, 0.463f });
	mesh.push_back({ -1.545f, 0.82f, 0.311f });
	mesh.push_back({ -2.025f, 0.82f, 0.343f });
	mesh.push_back({ -2.093f, 0.82f, 0.543f });
	mesh.push_back({ -2.021f, 0.82f, 0.635f });
	mesh.push_back({ -2.001f, 0.82f, 0.983f });
	mesh.push_back({ -1.357f, 0.6f, 1.367f });
	mesh.push_back({ 0.291f, 0.6f, 1.379f });
	mesh.push_back({ 1.043f, 0.82f, 0.935f });
	mesh.push_back({ 1.979f, 0.82f, 0.743f });
	mesh.push_back({ 2.079f, 0.82f, 0.519f });
	mesh.push_back({ -1.858f, 0.82f, 0.852f });
	mesh.push_back({ -1.196f, 0.82f, 0.838f });
	mesh.push_back({ -0.178f, 0.82f, 0.818f });
	mesh.push_back({ 0.775f, 0.82f, 0.812f });

	mesh.push_back({ 2.063f, -0.82f, 0.287f });
	mesh.push_back({ 1.623f, -0.82f, 0.255f });
	mesh.push_back({ 1.607f, -0.82f, 0.403f });
	mesh.push_back({ 1.503f, -0.82f, 0.551f });
	mesh.push_back({ 1.359f, -0.82f, 0.619f });
	mesh.push_back({ 1.159f, -0.82f, 0.607f });
	mesh.push_back({ 1.015f, -0.82f, 0.499f });
	mesh.push_back({ 0.951f, -0.82f, 0.279f });
	mesh.push_back({ -0.189f, -0.82f, 0.291f });
	mesh.push_back({ -0.881f, -0.82f, 0.243f });
	mesh.push_back({ -0.893f, -0.82f, 0.407f });
	mesh.push_back({ -1.005f, -0.82f, 0.551f });
	mesh.push_back({ -1.209f, -0.82f, 0.615f });
	mesh.push_back({ -1.385f, -0.82f, 0.567f });
	mesh.push_back({ -1.493f, -0.82f, 0.463f });
	mesh.push_back({ -1.545f, -0.82f, 0.311f });
	mesh.push_back({ -2.025f, -0.82f, 0.343f });
	mesh.push_back({ -2.093f, -0.82f, 0.543f });
	mesh.push_back({ -2.021f, -0.82f, 0.635f });
	mesh.push_back({ -2.001f, -0.82f, 0.983f });
	mesh.push_back({ -1.357f, -0.6f, 1.367f });
	mesh.push_back({ 0.291f, -0.6f, 1.379f });
	mesh.push_back({ 1.043f, -0.82f, 0.935f });
	mesh.push_back({ 1.979f, -0.82f, 0.743f });
	mesh.push_back({ 2.079f, -0.82f, 0.519f });
	mesh.push_back({ -1.858f, -0.82f, 0.852f });
	mesh.push_back({ -1.196f, -0.82f, 0.838f });
	mesh.push_back({ -0.178f, -0.82f, 0.818f });
	mesh.push_back({ 0.775f, -0.82f, 0.812f });



	triangles.push_back({  0,  1,  2, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  0,  2, 24, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  2, 23, 24, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  2,  3, 23, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  3, 22, 23, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  3,  4, 22, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  4,  5, 22, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  6,  7, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  6, 27, 28, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  7 , 8, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  8,  9, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  9, 10, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 10, 11, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 11, 26, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 11, 12, 26, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 20, 21, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 20, 26, 27, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 12, 25, 26, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 20, 25, 26, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 12, 13, 25, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 19, 20, 25, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 18, 19, 25, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 13, 18, 25, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 13, 14, 18, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 14, 15, 17, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 15, 16, 17, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 14, 17, 18, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  5, 22, 28, 0.0f, 1.0f, 0.0f });
	triangles.push_back({  5,  6, 28, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 21, 27, 28, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 21, 22, 28, 0.0f, 1.0f, 0.0f });


	triangles.push_back({ 29, 30, 31, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 29, 31, 53, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 31, 52, 53, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 31, 32, 52, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 32, 51, 52, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 32, 33, 51, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 33, 34, 51, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 35, 36, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 35, 56, 57, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 36, 37, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 37, 38, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 38, 39, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 39, 40, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 40, 55, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 40, 41, 55, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 49, 50, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 49, 55, 56, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 41, 54, 55, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 49, 54, 55, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 41, 42, 54, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 48, 49, 54, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 47, 48, 54, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 42, 47, 54, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 42, 43, 47, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 43, 44, 46, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 44, 45, 46, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 43, 46, 47, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 34, 51, 57, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 34, 35, 57, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 50, 56, 57, 0.0f, 1.0f, 0.0f });
	triangles.push_back({ 50, 51, 57, 0.0f, 1.0f, 0.0f });


	triangles.push_back({  0, 29, 24, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 29, 24, 53, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 24, 53, 23, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 53, 23, 52, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 23, 52, 22, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 52, 22, 51, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 22, 51, 21, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 51, 21, 50, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 21, 50, 20, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 50, 20, 49, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 20, 49, 19, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 49, 19, 48, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 19, 48, 18, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 48, 18, 47, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 18, 47, 17, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 47, 17, 46, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 17, 46, 16, 0.0f, 0.8f, 0.0f });
	triangles.push_back({ 46, 16, 45, 0.0f, 0.8f, 0.0f });

}

void Car::move()
{
	if (tryAccelerate)
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
	}

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

void Car::printWheels()
{
	for (auto& wheel : wheels)
	{
		wheel.printModel();
	}
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