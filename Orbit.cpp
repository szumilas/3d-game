#include "Orbit.h"
#include <string>

#include <fstream>

Orbit::Orbit()
{
	std::fstream file;
	file.open("Data/OrbitData.txt");
	if (file)
	{
		std::string line;
		std::getline(file, line);
		lookAtX = stof(line);
		std::getline(file, line);
		lookAtY = stof(line);
		std::getline(file, line);
		distance = stof(line);
		std::getline(file, line);
		rz = stof(line);
		std::getline(file, line);
		alpha = stof(line);
	}
	else
	{
		lookAtX = 0;
		lookAtY = 0;
		distance = 100;
		rz = 3.14;
		alpha = 3.14 / 4;
	}
	file.close();
	
	flatCursor = Point{ 400, 400, 0 };
}

void Orbit::savePosition()
{
	std::fstream file;
	file.open("Data/OrbitData.txt", std::ios::out);
	file << lookAtX << std::endl;
	file << lookAtY << std::endl;
	file << distance << std::endl;
	file << rz << std::endl;
	file << alpha << std::endl;
	file.close();
}

Point Orbit::getCameraCenter()
{
	Point eyePoint;
	eyePoint.x = lookAtX + (distance * cos(alpha) * cos(rz));
	eyePoint.y = lookAtY + (distance * cos(alpha) * sin(rz));
	eyePoint.z = distance * sin(alpha);
	return eyePoint;

}

Point Orbit::getCameraLookAt()
{
	Point lookAtPoint;
	lookAtPoint.x = lookAtX;
	lookAtPoint.y = lookAtY;
	lookAtPoint.z = 0;
	return lookAtPoint;

}

void Orbit::calculateFlatCursorPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle)
{
	calculateFlatCursorRealWorldPosition(windowWidth, windowHeight, mouseXpos, mouseYpos, angle);

	// v = [a, b, c]
	//	x = x0 + a * t
	//{	y = y0 + b * t
	//	z = z0 + c * t

	auto eyePoint = getCameraCenter();
	float a = flatCursorRealWorld.x - eyePoint.x;
	float b = flatCursorRealWorld.y - eyePoint.y;
	float c = flatCursorRealWorld.z - eyePoint.z;

	if (c != 0)
	{
		float t = -eyePoint.z / c;
		flatCursor.x = eyePoint.x + a * t;
		flatCursor.y = eyePoint.y + b * t;

		if (moveOrbitXY && !previouslyRendered)
		{
			lookAtX += (-flatCursor.x + previousFlatCursor.x)*2;
			lookAtY += (-flatCursor.y + previousFlatCursor.y)*2;
			previouslyRendered = true;
		}
		if (moveLookAt && !previouslyRenderedLookAt)
		{
			lookAtX += (-flatCursor.x + previousFlatCursor.x) * 2;
			lookAtY += (-flatCursor.y + previousFlatCursor.y) * 2;

			calculateNewRzAlphaDistance(eyePoint);

			previouslyRenderedLookAt = true;
		}
	}
}

void Orbit::calculateFlatCursorRealWorldPosition(int windowWidth, int windowHeight, int mouseXpos, int mouseYpos, int angle)
{
	auto eyePoint = getCameraCenter();

	float z = eyePoint.z;
	float r = sqrt(pow(eyePoint.x - lookAtX, 2) + pow(eyePoint.y - lookAtY, 2));

	float hr = tan((static_cast<float>(angle) / 180 * 3.14) / 2) * distance * 2;

	float realYDistanceOnScreen = (windowHeight / 2 - mouseYpos) * hr / windowHeight;
	float realXDistanceOnScreen = (windowWidth / 2 - mouseXpos) * hr / windowHeight;

	flatCursorRealWorld.x = lookAtX + realXDistanceOnScreen * sin(rz)	-	 realYDistanceOnScreen * cos(atan(r / z)) * cos(rz);
	flatCursorRealWorld.y = lookAtY - realXDistanceOnScreen * cos(rz)	-	 realYDistanceOnScreen * cos(atan(r / z)) * sin(rz);
	flatCursorRealWorld.z = realYDistanceOnScreen * sin(atan(r / z));

}

void Orbit::displayFlatCursor()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(getFlatCursorX() - 5, getFlatCursorY(), 0);
	glVertex3f(getFlatCursorX() + 5, getFlatCursorY(), 0);
	glVertex3f(getFlatCursorX(), getFlatCursorY() - 5, 0);
	glVertex3f(getFlatCursorX(), getFlatCursorY() + 5, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(getLookAtX() - 1, getLookAtY(), 0);
	glVertex3f(getLookAtX() + 1, getLookAtY(), 0);
	glVertex3f(getLookAtX(), getLookAtY() - 1, 0);
	glVertex3f(getLookAtX(), getLookAtY() + 1, 0);
	glEnd();
}

void Orbit::cameraCenterUp()
{
	auto eyePoint = getCameraCenter();

	if (eyePoint.z < 5)
		eyePoint.z *= 1.03;
	else
		eyePoint.z += 0.5;

	calculateNewRzAlphaDistance(eyePoint);
}

void Orbit::cameraCenterDown()
{
	auto eyePoint = getCameraCenter();

	if (eyePoint.z < 5)
		eyePoint.z /= 1.03;
	else
		eyePoint.z -= 0.5;

	if (eyePoint.z < 0.1)
		eyePoint.z = 0.1;

	calculateNewRzAlphaDistance(eyePoint);
}

void Orbit::calculateNewRzAlphaDistance(const Point& eyePoint)
{
	auto newDistance = eyePoint.distance3D({ lookAtX, lookAtY });
	auto newAlpha = asin(eyePoint.z / newDistance);
	vector2D line({ lookAtX, lookAtY }, eyePoint);
	auto newrz = vector2D::directedAngle({ { 0,0 },{ 1,0 } }, line );
	
	rz = newrz;
	alpha = newAlpha;
	distance = newDistance;
}