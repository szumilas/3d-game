#include "Wheel.h"

Wheel::Wheel()
{

	X = 20;
	Y = -16;
	Z = 0.01;

	rz = 0;
}

void Wheel::loadModel()
{
	mesh.push_back({    0.0f, 0.09f,   0.0f });
	mesh.push_back({  0.152f, 0.09f, 0.053f });
	mesh.push_back({  0.246f, 0.09f, 0.151f });
	mesh.push_back({  0.268f, 0.09f, 0.272f });
	mesh.push_back({  0.244f, 0.09f, 0.404f });
	mesh.push_back({  0.143f, 0.09f, 0.511f });
	mesh.push_back({ -0.006f, 0.09f, 0.556f });
	mesh.push_back({ -0.148f, 0.09f, 0.515f });
	mesh.push_back({ -0.249f, 0.09f, 0.419f });
	mesh.push_back({ -0.287f, 0.09f, 0.282f });
	mesh.push_back({ -0.249f, 0.09f, 0.142f });
	mesh.push_back({ -0.145f, 0.09f, 0.042f });
	mesh.push_back({ -0.006f, 0.09f, 0.098f });
	mesh.push_back({    0.1f, 0.09f, 0.134f });
	mesh.push_back({  0.157f, 0.09f, 0.207f });
	mesh.push_back({  0.174f, 0.09f, 0.272f });
	mesh.push_back({  0.154f, 0.09f, 0.357f });
	mesh.push_back({  0.086f, 0.09f, 0.431f });
	mesh.push_back({ -0.009f, 0.09f, 0.453f });
	mesh.push_back({ -0.109f, 0.09f, 0.421f });
	mesh.push_back({ -0.165f, 0.09f, 0.351f });
	mesh.push_back({  -0.18f, 0.09f, 0.278f });
	mesh.push_back({ -0.154f, 0.09f,  0.18f });
	mesh.push_back({ -0.094f, 0.09f, 0.123f });
	mesh.push_back({ -0.004f, 0.09f, 0.275f });

	
	mesh.push_back({    0.0f, -0.09f,   0.0f });
	mesh.push_back({  0.152f, -0.09f, 0.053f });
	mesh.push_back({  0.246f, -0.09f, 0.151f });
	mesh.push_back({  0.268f, -0.09f, 0.272f });
	mesh.push_back({  0.244f, -0.09f, 0.404f });
	mesh.push_back({  0.143f, -0.09f, 0.511f });
	mesh.push_back({ -0.006f, -0.09f, 0.556f });
	mesh.push_back({ -0.148f, -0.09f, 0.515f });
	mesh.push_back({ -0.249f, -0.09f, 0.419f });
	mesh.push_back({ -0.287f, -0.09f, 0.282f });
	mesh.push_back({ -0.249f, -0.09f, 0.142f });
	mesh.push_back({ -0.145f, -0.09f, 0.042f });
	mesh.push_back({ -0.006f, -0.09f, 0.098f });
	mesh.push_back({    0.1f, -0.09f, 0.134f });
	mesh.push_back({  0.157f, -0.09f, 0.207f });
	mesh.push_back({  0.174f, -0.09f, 0.272f });
	mesh.push_back({  0.154f, -0.09f, 0.357f });
	mesh.push_back({  0.086f, -0.09f, 0.431f });
	mesh.push_back({ -0.009f, -0.09f, 0.453f });
	mesh.push_back({ -0.109f, -0.09f, 0.421f });
	mesh.push_back({ -0.165f, -0.09f, 0.351f });
	mesh.push_back({  -0.18f, -0.09f, 0.278f });
	mesh.push_back({ -0.154f, -0.09f,  0.18f });
	mesh.push_back({ -0.094f, -0.09f, 0.123f });
	mesh.push_back({ -0.004f, -0.09f, 0.275f });



	triangles.push_back({  0,  1, 12 });
	triangles.push_back({  1,  2, 13 });
	triangles.push_back({  2,  3, 14 });
	triangles.push_back({  3,  4, 15 });
	triangles.push_back({  4,  5, 16 });
	triangles.push_back({  5,  6, 17 });
	triangles.push_back({  6,  7, 18 });
	triangles.push_back({  7,  8, 19 });
	triangles.push_back({  8,  9, 20 });
	triangles.push_back({  9, 10, 21 });
	triangles.push_back({ 10, 11, 22 });
	triangles.push_back({ 11,  0, 23 });

	triangles.push_back({ 12, 13, 1 });
	triangles.push_back({ 13, 14, 2 });
	triangles.push_back({ 14, 15, 3 });
	triangles.push_back({ 15, 16, 4 });
	triangles.push_back({ 16, 17, 5 });
	triangles.push_back({ 17, 18, 6 });
	triangles.push_back({ 18, 19, 7 });
	triangles.push_back({ 19, 20, 8 });
	triangles.push_back({ 20, 21, 9 });
	triangles.push_back({ 21, 22, 10 });
	triangles.push_back({ 22, 23, 11 });
	triangles.push_back({ 23, 12, 0 });

	triangles.push_back({ 12, 13, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 13, 14, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 14, 15, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 15, 16, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 16, 17, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 17, 18, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 18, 19, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 19, 20, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 20, 21, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 21, 22, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 22, 23, 24, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 23, 12, 24, 0.5f, 0.5f, 0.5f });

	//-----

	triangles.push_back({ 25, 26, 37 });
	triangles.push_back({ 26, 27, 38 });
	triangles.push_back({ 27, 28, 39 });
	triangles.push_back({ 28, 29, 40 });
	triangles.push_back({ 29, 30, 41 });
	triangles.push_back({ 30, 31, 42 });
	triangles.push_back({ 31, 32, 43 });
	triangles.push_back({ 32, 33, 44 });
	triangles.push_back({ 33, 34, 45 });
	triangles.push_back({ 34, 35, 46 });
	triangles.push_back({ 35, 36, 47 });
	triangles.push_back({ 36, 25, 48 });

	triangles.push_back({ 37, 38, 26 });
	triangles.push_back({ 38, 39, 27 });
	triangles.push_back({ 39, 40, 28 });
	triangles.push_back({ 40, 41, 29 });
	triangles.push_back({ 41, 42, 30 });
	triangles.push_back({ 42, 43, 31 });
	triangles.push_back({ 43, 44, 32 });
	triangles.push_back({ 44, 45, 33 });
	triangles.push_back({ 45, 46, 34 });
	triangles.push_back({ 46, 47, 35 });
	triangles.push_back({ 47, 48, 36 });
	triangles.push_back({ 48, 37, 25 });

	triangles.push_back({ 37, 38, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 38, 39, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 39, 40, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 40, 41, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 41, 42, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 42, 43, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 43, 44, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 44, 45, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 45, 46, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 46, 47, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 47, 48, 49, 0.5f, 0.5f, 0.5f });
	triangles.push_back({ 48, 37, 49, 0.5f, 0.5f, 0.5f });

}


void Wheel::adjustPosition(float Xcar, float Ycar, float rzCar, float xOffset, float yOffset, float steeringWheelAngle)
{
	float sin_rzCar = sin(rzCar);
	float cos_rzCar = cos(rzCar);

	rz = rzCar + steeringWheelAngle;

	X = Xcar + xOffset * cos_rzCar - yOffset * sin_rzCar;
	Y = Ycar + xOffset * sin_rzCar + yOffset * cos_rzCar;


}