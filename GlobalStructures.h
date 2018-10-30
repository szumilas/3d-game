#pragma once

#define FPS 60

struct Point
{
	float x;
	float y;
	float z;

	Point() { x = 0; y = 0; z = 0; }
	Point(float x, float y, float z) { Point::x = x; Point::y = y; Point::z = z; }
	Point(float x, float y) { Point::x = x; Point::y = y; z = 0; }
	
	float distance2D(Point& second)
	{
		return sqrt(pow(x - second.x, 2) + pow(y - second.y, 2));
	}

	bool operator==(const Point& second) { return x == second.x && y == second.y && z == second.z; }
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

	Point calcuateIntersectionPoint(Line2D& secondLine)
	{
		Point intersection;
		float W = A * secondLine.B - B * secondLine.A;
		float Wx = -C * secondLine.B + B * secondLine.C;
		float Wy = -A * secondLine.C + C * secondLine.A;

		if (W == 0)
		{
			if (B == 0)
			{
				intersection.x = -C / A;
				intersection.y = 0;
			}
			else
			{
				intersection.x = 1000000;
				intersection.y = (-1000000 * A - C) / B;
			}
		}
		else
		{
			intersection.x = Wx / W;
			intersection.y = Wy / W;
		}

		return intersection;
	}

	bool isSame(Line2D& secondLine)
	{
		float delta = 0.1;
		if (abs(A - secondLine.A) < delta && abs(B - secondLine.B) < delta && abs(C - secondLine.C) < delta)
			return true;
		else
			return false;
	}

	float pointDistance(const Point& point)
	{
		return abs(A * point.x + B * point.y + C) / (sqrt(A * A + B *B));
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