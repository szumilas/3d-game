#pragma once

#define FPS 60

struct Point
{
	float x;
	float y;
	float z;

	float distance(Point& second)
	{
		return sqrt(pow(x - second.x, 2) + pow(y - second.y, 2));
	}
};

struct Line2D
{
	//Ax + By + C = 0
	float A;
	float B;
	float C;

	Line2D() { A = 0; B = 0; C = 0; };
	Line2D(float A, float B, float C) : A(A), B(B), C(C) {};
	Line2D(Point& first, Point& second)
	{
		if (first.x == second.x)
		{
			A = 1.0f;
			B = 0.0f;
			C = -first.x;
		}
		else
		{
			A = (second.y - first.y) / (first.x - second.x);
			B = 1;
			C = -((second.y - first.y) / (first.x - second.x)) * first.x - first.y;
		}
	}

	void calculateBisector(Line2D& firstLine, Line2D& secondLine, bool secondSolution = false)
	{
		float a = firstLine.A;
		float b = firstLine.B;
		float c = firstLine.C;
		float d = secondLine.A;
		float e = secondLine.B;
		float f = secondLine.C;
		float g = sqrt(pow(a, 2) + pow(b, 2)) / sqrt(pow(d, 2) + pow(e, 2));

		if (secondSolution)
		{
			A = a - d * g;
			B = b - e * g;
			C = c - f * g;
		}
		else
		{
			A = a + d * g;
			B = b + e * g;
			C = c + f * g;
		}
	}
};

struct node
{
	long long id;
	double lon;
	double lat;

	float posX;
	float posY;
};