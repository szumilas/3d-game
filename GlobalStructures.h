#pragma once

#include <cmath>
#include <vector>
#include "enum.h"

#define FPS 30
#define PI 3.1415926535
#define GRAVITY 9.81

class Color
{
	struct ColorData
	{
		ColorName colorName;
		float red;
		float green;
		float blue;
	};

public:

	Color();
	Color(ColorName colorName);
	Color(float red, float green, float blue);

	Color mixColor(Color& second);
	
	float red;
	float green;
	float blue;

private:
	std::vector<ColorData> colors{
		{ ColorName::WHITE, 1.0f, 1.0f, 1.0f },
		{ ColorName::BLACK, 0.0f, 0.0f, 0.0f },
		{ ColorName::LIGHT_GRAY, 0.8f, 0.8f, 0.8f },
		{ ColorName::DARK_GRAY, 0.2f, 0.2f, 0.2f },
		{ ColorName::GRAY, 0.5f, 0.5f, 0.5f },
		{ ColorName::RED, 1.0f, 0.0f, 0.0f },
		{ ColorName::GREEN, 0.0f, 1.0f, 0.0f },
		{ ColorName::BLUE, 0.0f, 0.0f, 1.0f },
		{ ColorName::ORANGE, 1.0f, 0.5f, 0.0f },
		{ ColorName::BROWN, 0.5f, 0.25f, 0.0f },
		{ ColorName::YELLOW, 1.0f, 1.0f, 0.0f },
		{ ColorName::PINK, 1.0f, 0.0f, 0.5f },
	};
};

struct Point
{
	double x;
	double y;
	double z;

	Point();
	Point(double x, double y, double z);
	Point(double x, double y);
	
	double distance2D(const Point& second) const;
	static void rotate(Point& p, const Point& refP, float angle);
	void move2D(double dx, double dy);

	bool operator==(const Point& second) const;
	bool operator!=(const Point& second) const;
	bool operator<(const Point& second) const;
	Point operator+(const Point& p2) const;
};

struct Circle
{
	Point center;
	float r;

	bool isColliding(const Circle& with);
};

struct Line2D
{
	//Ax + By + C = 0
	float A;
	float B;
	float C;

	Line2D();
	Line2D(float A, float B, float C);
	Line2D(Point& first, Point& second);

	void calculateBisector(Line2D& firstLine, Line2D& secondLine, bool secondSolution = false);
	Point calcuateIntersectionPoint(Line2D& secondLine);
	bool isSame(Line2D& secondLine);
	float pointDistance(const Point& point);
};

struct Plane
{
	Plane();
	Plane(Point& p1, Point& p2, Point& p3);

	float A, B, C, D;
};

struct vector2D
{
	vector2D();
	vector2D(double x, double y);
	vector2D(const Point& p1, const Point& p2);
	vector2D(vector2D& v1, vector2D& v2);
	vector2D operator+(const vector2D& v2) const;

	void operator+=(const vector2D& v2);
	void operator*=(double scale);
	void negate();
	double length() const;
	static double crossProduct(const vector2D& v1, const vector2D& v2);
	static double dotProduct(const vector2D& v1, const vector2D& v2);
	static double angle(const vector2D& v1, const vector2D& v2);
	static double realAngle(const vector2D& v1, const vector2D& v2);
	static double directedAngle(const vector2D& v1, const vector2D& v2);
	void convertIntoUnitVector();
	void rotate(double angle);

	double x = 0.0;
	double y = 0.0;

};

struct node
{
	long long id;
	double lon;
	double lat;

	float posX;
	float posY;
};

struct Force
{
	Force();
	Force(Point center, vector2D vector);

	Point center;
	vector2D vector;
};