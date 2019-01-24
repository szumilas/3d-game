#pragma once

#define FPS 60
#define PI 3.1415926535

struct Color
{
	float red;
	float green;
	float blue;

	Color mixColor(Color& second) { return Color{(red + second.red) / 2, (green + second.green) / 2 , (blue + second.blue) / 2 }; }
	static Color white() { return Color{ 1.0f, 1.0f, 1.0f }; }
	static Color lightGray() { return Color{ 0.8f, 0.8f, 0.8f }; }
	static Color gray() { return Color{ 0.5f, 0.5f, 0.5f }; }
};

struct Point
{
	double x;
	double y;
	double z;

	Point() { x = 0; y = 0; z = 0; }
	Point(double x, double y, double z) { Point::x = x; Point::y = y; Point::z = z; }
	Point(double x, double y) { Point::x = x; Point::y = y; z = 0; }
	
	double distance2D(const Point& second) const
	{
		return sqrt(pow(x - second.x, 2) + pow(y - second.y, 2));
	}

	bool operator==(const Point& second) const { return x == second.x && y == second.y && z == second.z; }
	bool operator!=(const Point& second) const { return x != second.x || y != second.y || z != second.z; }
	bool operator<(const Point& second) const { return x < second.x || x == second.x && y < second.y || x == second.x && y == second.y && z < second.z; }
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

struct Plane
{
	Plane() {};
	Plane(Point& p1, Point& p2, Point& p3)
	{
		auto detA = [](float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33 ) {
			return a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - (a13*a22*a31 + a11*a23*a32 + a12*a21*a33);
		};

		
		float WD = detA(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
		float WA = detA(1.0f, p1.y, p1.z, 1.0f, p2.y, p2.z, 1.0f, p3.y, p3.z);
		float WB = detA(p1.x, 1.0f, p1.z, p2.x, 1.0f, p2.z, p3.x, 1.0f, p3.z);
		float WC = detA(p1.x, p1.y, 1.0f, p2.x, p2.y, 1.0f, p3.x, p3.y, 1.0f);

		D = 1;
		A = -D * WA / WD;
		B = -D * WB / WD;
		C = -D * WC / WD;
	}

	float A, B, C, D;
};

struct vector2D
{
	vector2D(Point& p1, Point& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
	}

	vector2D(vector2D& v1, vector2D& v2)
	{
		x = v1.x + v2.x;
		y = v1.y + v2.y;
	}

	void negate()
	{
		x *= -1.0f;
		y *= -1.0f;
	}

	double length() const
	{
		return sqrt(x * x + y * y);
	}

	static double crossProduct(const vector2D& v1, const vector2D& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	static double dotProduct(const vector2D& v1, const vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static double angle(const vector2D& v1, const vector2D& v2)
	{
		return acos(dotProduct(v1, v2) / (v1.length() * v2.length()));
	}

	static double realAngle(const vector2D& v1, const vector2D& v2)
	{
		auto alpha = angle(v1, v2);

		if (v1.x < 0 && v1.y < 0)
			alpha = 2 * PI - alpha;
		else if (v1.x > 0 && v1.y < 0)
			alpha = 2 * PI - alpha;

		return alpha;
	}

	void convertIntoUnitVector()
	{
		double dist = sqrt(x * x + y * y);
		x /= dist;
		y /= dist;
	}

	double x;
	double y;

};

struct node
{
	long long id;
	double lon;
	double lat;

	float posX;
	float posY;
};