#include "Object3D.h"

Object3D::Object3D()
{


}

void Object3D::loadModel()
{
	



}

void Object3D::printModel()
{
	sin_rz = sin(rz);
	cos_rz = cos(rz);

	int q = 0;

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);

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