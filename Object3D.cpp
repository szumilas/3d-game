#include "Object3D.h"

Object3D::Object3D()
{
	X = 15;
	Y = -16;
	Z = 0.01;

	rz = 3.14/2;
}

void Object3D::loadModel(std::string path)
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



	triangles.push_back({ 0, 1, 2 });
	triangles.push_back({ 0, 2, 24 });
	triangles.push_back({ 2, 23, 24 });
	triangles.push_back({ 2, 3, 23 });
	triangles.push_back({ 3, 22, 23 });
	triangles.push_back({ 3, 4, 22 });
	triangles.push_back({ 4, 5, 22 });
	triangles.push_back({ 6, 7, 27 });
	triangles.push_back({ 6, 27, 28 });
	triangles.push_back({ 7, 8, 27 });
	triangles.push_back({ 8, 9, 27 });
	triangles.push_back({ 9, 10, 27 });
	triangles.push_back({ 10, 11, 27 });
	triangles.push_back({ 11, 26, 27 });
	triangles.push_back({ 11, 12, 26 });
	triangles.push_back({ 20, 21, 27 });
	triangles.push_back({ 20, 26, 27 });
	triangles.push_back({ 12, 25, 26 });
	triangles.push_back({ 20, 25, 26 });
	triangles.push_back({ 12, 13, 25 });
	triangles.push_back({ 19, 20, 25 });
	triangles.push_back({ 18, 19, 25 });
	triangles.push_back({ 13, 18, 25 });
	triangles.push_back({ 13, 14, 18 });
	triangles.push_back({ 14, 15, 17 });
	triangles.push_back({ 15, 16, 17 });
	triangles.push_back({ 14, 17, 18 });
	triangles.push_back({ 5, 22, 28 });
	triangles.push_back({ 5, 6, 28 });
	triangles.push_back({ 21, 27, 28 });
	triangles.push_back({ 21, 22, 28 });


	triangles.push_back({ 29, 30, 31 });
	triangles.push_back({ 29, 31, 53 });
	triangles.push_back({ 31, 52, 53 });
	triangles.push_back({ 31, 32, 52 });
	triangles.push_back({ 32, 51, 52 });
	triangles.push_back({ 32, 33, 51 });
	triangles.push_back({ 33, 34, 51 });
	triangles.push_back({ 35, 36, 56 });
	triangles.push_back({ 35, 56, 57 });
	triangles.push_back({ 36, 37, 56 });
	triangles.push_back({ 37, 38, 56 });
	triangles.push_back({ 38, 39, 56 });
	triangles.push_back({ 39, 40, 56 });
	triangles.push_back({ 40, 55, 56 });
	triangles.push_back({ 40, 41, 55 });
	triangles.push_back({ 49, 50, 56 });
	triangles.push_back({ 49, 55, 56 });
	triangles.push_back({ 41, 54, 55 });
	triangles.push_back({ 49, 54, 55 });
	triangles.push_back({ 41, 42, 54 });
	triangles.push_back({ 48, 49, 54 });
	triangles.push_back({ 47, 48, 54 });
	triangles.push_back({ 42, 47, 54 });
	triangles.push_back({ 42, 43, 47 });
	triangles.push_back({ 43, 44, 46 });
	triangles.push_back({ 44, 45, 46 });
	triangles.push_back({ 43, 46, 47 });
	triangles.push_back({ 34, 51, 57 });
	triangles.push_back({ 34, 35, 57 });
	triangles.push_back({ 50, 56, 57 });
	triangles.push_back({ 50, 51, 57 });


	triangles.push_back({ 0, 29, 24 });
	triangles.push_back({ 29, 24, 53 });
	triangles.push_back({ 24, 53, 23 });
	triangles.push_back({ 53, 23, 52 });
	triangles.push_back({ 23, 52, 22 });
	triangles.push_back({ 52, 22, 51 });
	triangles.push_back({ 22, 51, 21 });
	triangles.push_back({ 51, 21, 50 });
	triangles.push_back({ 21, 50, 20 });
	triangles.push_back({ 50, 20, 49 });
	triangles.push_back({ 20, 49, 19 });
	triangles.push_back({ 49, 19, 48 });
	triangles.push_back({ 19, 48, 18 });
	triangles.push_back({ 48, 18, 47 });
	triangles.push_back({ 18, 47, 17 });
	triangles.push_back({ 47, 17, 46 });
	triangles.push_back({ 17, 46, 16 });
	triangles.push_back({ 46, 16, 45 });



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
		if(q >= 62)
			glColor3f(0.7, 0, 0);

		glVertex3f(X + mesh[triangle.p1].x * cos_rz - mesh[triangle.p1].y * sin_rz, Y + mesh[triangle.p1].x * sin_rz + mesh[triangle.p1].y * cos_rz, Z + mesh[triangle.p1].z);
		glVertex3f(X + mesh[triangle.p2].x * cos_rz - mesh[triangle.p2].y * sin_rz, Y + mesh[triangle.p2].x * sin_rz + mesh[triangle.p2].y * cos_rz, Z + mesh[triangle.p2].z);
		glVertex3f(X + mesh[triangle.p3].x * cos_rz - mesh[triangle.p3].y * sin_rz, Y + mesh[triangle.p3].x * sin_rz + mesh[triangle.p3].y * cos_rz, Z + mesh[triangle.p3].z);

		q++;
	}

	glEnd();

}