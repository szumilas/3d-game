#include "Object3D.h"

Object3D::Object3D()
{


}

void Object3D::loadModel()
{
	
}

void Object3D::display()
{
	if (!alreadyPrinted)
	{
		alreadyPrinted = true;

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
				glVertex3f(polygon.points[i].x, polygon.points[i].y, polygon.points[i].z);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

void Object3D::printModel()
{
	sin_rz = sin(rz);
	cos_rz = cos(rz);

	int q = 0;

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);

	for (auto& triangle : triangles)
	{
		glColor3f(triangle.red, triangle.green, triangle.blue);

		glVertex3f(X + mesh[triangle.p1].x * cos_rz - mesh[triangle.p1].y * sin_rz, Y + mesh[triangle.p1].x * sin_rz + mesh[triangle.p1].y * cos_rz, Z + mesh[triangle.p1].z);
		glVertex3f(X + mesh[triangle.p2].x * cos_rz - mesh[triangle.p2].y * sin_rz, Y + mesh[triangle.p2].x * sin_rz + mesh[triangle.p2].y * cos_rz, Z + mesh[triangle.p2].z);
		glVertex3f(X + mesh[triangle.p3].x * cos_rz - mesh[triangle.p3].y * sin_rz, Y + mesh[triangle.p3].x * sin_rz + mesh[triangle.p3].y * cos_rz, Z + mesh[triangle.p3].z);

		q++;
	}

	glEnd();

}