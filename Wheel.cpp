#include "Wheel.h"

Wheel::Wheel()
{
	rz = 0.0f;
}

void Wheel::display()
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

			float firstX = polygon.points[i].x * cos(angle) - (polygon.points[i].z - r) * sin(angle);
			float firstZ = polygon.points[i].x * sin(angle) + (polygon.points[i].z - r) * cos(angle) + r;

			float secondX = firstX * cos(rz) - polygon.points[i].y * sin(rz);
			float secondY = firstX * sin(rz) + polygon.points[i].y * cos(rz);
			float secondZ = firstZ;

			toPrint.x = carX + (secondX + xOffset) * car_cos_rz - (secondY + yOffset) * car_sin_rz;
			toPrint.y = carY + (secondX + xOffset) * car_sin_rz + (secondY + yOffset) * car_cos_rz;
			toPrint.z = carZ + secondZ;


			glVertex3f(toPrint.x, toPrint.y, toPrint.z);
		}
		glEnd();
	}
}

void Wheel::calculateGeometry(float newXoffset, float newYoffset)
{
	xOffset = newXoffset;
	yOffset = newYoffset;

	for (auto& polygon : polygons)
	{
		for (auto& point : polygon.points)
		{
			if (point.z > r * 2)
				r = point.z / 2;
		}
	}
}

void Wheel::rotate(float v)
{
	angle -= v / r / FPS;
}