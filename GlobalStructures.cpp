#include "GlobalStructures.h"

#include <string>
#include <sstream>

std::vector<Color::ColorData> Color::colors{
	{ ColorName::WHITE, 1.0f, 1.0f, 1.0f },
	{ ColorName::BLACK, 0.0f, 0.0f, 0.0f },
	{ ColorName::LIGHT_GRAY, 0.8f, 0.8f, 0.8f },
	{ ColorName::DARK_GRAY, 0.2f, 0.2f, 0.2f },
	{ ColorName::GRAY, 0.5f, 0.5f, 0.5f },
	{ ColorName::MENU_GRAY, 0.2f, 0.23f, 0.23f },
	{ ColorName::MENU_LIGHT_GRAY, 0.37f, 0.44f, 0.44f },
	{ ColorName::MENU_DARK_GRAY, 0.15f, 0.17f, 0.18f },
	{ ColorName::MENU_BLUE, 0.176f, 0.749f, 0.851f },
	{ ColorName::RED, 1.0f, 0.0f, 0.0f },
	{ ColorName::GREEN, 0.0f, 1.0f, 0.0f },
	{ ColorName::BLUE, 0.0f, 0.0f, 1.0f },
	{ ColorName::ORANGE, 1.0f, 0.5f, 0.0f },
	{ ColorName::BROWN, 0.5f, 0.25f, 0.0f },
	{ ColorName::YELLOW, 1.0f, 1.0f, 0.0f },
	{ ColorName::PINK, 1.0f, 0.0f, 0.5f },
	{ ColorName::VIOLET, 0.62f, 0.0f, 0.72f },
};

Color::Color()
{
	red = 0.0f;
	green = 0.0f;
	blue = 0.0f;
}

Color::Color(ColorName colorName)
{
	red = colors[static_cast<long>(colorName)].red;
	green = colors[static_cast<long>(colorName)].green;
	blue = colors[static_cast<long>(colorName)].blue;
};

Color::Color(float red, float green, float blue) : 
	red(red), green(green), blue(blue)
{

}

Color Color::mixColor(Color& second)
{
	return Color((red + second.red) / 2, (green + second.green) / 2, (blue + second.blue) / 2);
}

std::string Color::getColorHex()
{
	std::string hexColor = "#";
	std::stringstream ssr, ssg, ssb;

	ssr << std::hex << static_cast<int>(red * 255 + 256);
	hexColor += ssr.str().substr(1, 2);

	ssg << std::hex << static_cast<int>(green * 255 + 256);
	hexColor += ssg.str().substr(1, 2);

	ssb << std::hex << static_cast<int>(blue * 255 + 256);
	hexColor += ssb.str().substr(1, 2);

	return hexColor;
}

Point::Point()
{
	x = 0;
	y = 0;
	z = 0;
}

Point::Point(double x, double y, double z)
{
	Point::x = x;
	Point::y = y;
	Point::z = z;
}

Point::Point(double x, double y)
{ 
	Point::x = x;
	Point::y = y;
	z = 0; 
}

double Point::distance2D(const Point& second) const
{
	return sqrt(pow(x - second.x, 2) + pow(y - second.y, 2));
}

double Point::distance3D(const Point& second) const
{
	return sqrt(pow(x - second.x, 2) + pow(y - second.y, 2) + pow(z - second.z, 2));
}

void Point::rotate(Point& p, const Point& refP, float angle)
{
	double newX = refP.x + (p.x - refP.x) * cos(angle) - (p.y - refP.y) * sin(angle);
	double newY = refP.y + (p.y - refP.y) * cos(angle) + (p.x - refP.x) * sin(angle);
	p.x = newX;
	p.y = newY;
}

void Point::move2D(double dx, double dy)
{
	x += dx;
	y += dy;
}

bool Point::operator==(const Point& second) const 
{
	return x == second.x && y == second.y && z == second.z; 
}

bool Point::operator!=(const Point& second) const
{
	return x != second.x || y != second.y || z != second.z; 
}

bool Point::operator<(const Point& second) const 
{
	return x < second.x || x == second.x && y < second.y || x == second.x && y == second.y && z < second.z;
}

Point Point::operator+(const Point& p2) const
{
	return { x + p2.x, y + p2.y, z + p2.z };
}

Point& Point::operator+=(const Point& p2)
{
	x += p2.x;
	y += p2.y;
	z += p2.z;

	return *this;
}

Point& Point::operator-=(const Point& p2)
{
	x -= p2.x;
	y -= p2.y;
	z -= p2.z;

	return *this;
}

bool Circle::isColliding(const Circle& with)
{
	return center.distance2D(with.center) < r + with.r;
}

Line2D::Line2D() 
{ 
	A = 0;
	B = 0; 
	C = 0; 
};

Line2D::Line2D(float A, float B, float C)
	: A(A), B(B), C(C) 
{

};

Line2D::Line2D(const Point& first, const Point& second)
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

void Line2D::calculateBisector(Line2D& firstLine, Line2D& secondLine, bool secondSolution)
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

Point Line2D::calcuateIntersectionPoint(Line2D& secondLine)
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

bool Line2D::isSame(Line2D& secondLine)
{
	float delta = 0.1;
	if (abs(A - secondLine.A) < delta && abs(B - secondLine.B) < delta && abs(C - secondLine.C) < delta)
		return true;
	else
		return false;
}

float Line2D::pointDistance(const Point& point)
{
	return abs(A * point.x + B * point.y + C) / (sqrt(A * A + B *B));
}

Plane::Plane() 
{

};

Plane::Plane(Point& p1, Point& p2, Point& p3)
{
	auto detA = [](float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33) {
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

vector2D::vector2D()
{
	x = 0.0;
	y = 0.0;
}

vector2D::vector2D(double x, double y) : x(x), y(y)
{

}

vector2D::vector2D(const Point& p1, const Point& p2)
{
	x = p2.x - p1.x;
	y = p2.y - p1.y;
}

vector2D::vector2D(vector2D& v1, vector2D& v2)
{
	x = v1.x + v2.x;
	y = v1.y + v2.y;
}

vector2D vector2D::operator+(const vector2D& v2) const
{
	return { x + v2.x, y + v2.y };
}

void vector2D::operator+=(const vector2D& v2)
{
	x += v2.x;
	y += v2.y;
}

void vector2D::operator*=(double scale)
{
	x *= scale;
	y *= scale;
}

void vector2D::negate()
{
	x *= -1.0f;
	y *= -1.0f;
}

double vector2D::length() const
{
	return sqrt(x * x + y * y);
}

double vector2D::crossProduct(const vector2D& v1, const vector2D& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

double vector2D::dotProduct(const vector2D& v1, const vector2D& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

double vector2D::angle(const vector2D& v1, const vector2D& v2)
{
	auto argument = dotProduct(v1, v2) / (v1.length() * v2.length());
	if (argument > 1.0)
		argument = 1.0;
	else if (argument < -1.0)
		argument = -1.0;
	return acos(argument);
}

double vector2D::realAngle(const vector2D& v1, const vector2D& v2)
{
	auto alpha = angle(v1, v2);

	if (v1.x < 0 && v1.y < 0)
		alpha = 2 * PI - alpha;
	else if (v1.x > 0 && v1.y < 0)
		alpha = 2 * PI - alpha;

	return alpha;
}

double vector2D::directedAngle(const vector2D& v1, const vector2D& v2)
{
	auto alpha = angle(v1, v2);

	if (crossProduct(v1, v2) < 0)
		alpha = 2 * PI - alpha;

	return alpha;
}

void vector2D::convertIntoUnitVector()
{
	double dist = sqrt(x * x + y * y);
	x /= dist;
	y /= dist;
}

void vector2D::rotate(double angle)
{
	double x2 = x * cos(angle) - y * sin(angle);
	y = x * sin(angle) + y * cos(angle);
	x = x2;
}

Force::Force()
{

}

Force::Force(Point center, vector2D vector) : center(center), vector(vector)
{

}

Point Spline::getSplineSubpoint(float t)
{
	if (size() > 3)
	{
		int p0, p1, p2, p3;

		p1 = static_cast<int>(t) + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;


		t = t - static_cast<int>(t);

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f*tt - t;
		float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
		float q3 = -3.0f*ttt + 4.0f*tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);
		float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

		return Point(tx, ty, tz);
	}
	else
		return Point();
}

float Spline::length()
{
	float length = 0.0f;
	for (auto& segmentLength : lengths)
	{
		length += segmentLength;
	}

	return length;
}

void Spline::push_back(const Point& point)
{
	points.push_back(point);
}
unsigned int Spline::size()
{
	return points.size();
}

void Spline::calculateLengths()
{
	lengths.clear();
	for (int q = 0; q < size(); q++)
	{
		if (q > 0 && q < size() - 2)
		{
			lengths.push_back(calculateSegmentLength(q - 1));
		}
		else
		{
			lengths.push_back(0);
		}
	}
}

float Spline::calculateSegmentLength(int index)
{
	float segmentLength = 0.0f;
	float stepSize = 0.005;

	Point oldPoint, newPoint;
	oldPoint = getSplineSubpoint(static_cast<float>(index));

	for (float t = 0; t < 1.0f; t += stepSize)
	{
		newPoint = getSplineSubpoint(static_cast<float>(index) + t);
		segmentLength += sqrtf((newPoint.x - oldPoint.x)*(newPoint.x - oldPoint.x)
			+ (newPoint.y - oldPoint.y)*(newPoint.y - oldPoint.y));
		oldPoint = newPoint;
	}

	return segmentLength;
}

float Spline::getNormalisedOffset(float p)
{
	// Which node is the base?
	int i = 0;
	while (p > lengths[i + 1])
	{
		p -= lengths[i + 1];
		i++;
	}

	// The fractional is the offset 
	return static_cast<int>(i) + (p / lengths[i + 1]);
}

std::vector<Point> Spline::generateSubpoints(float subpointsDistance, bool keepOriginalPoints)
{
	std::vector<Point> subsplinePath;

	int previousIndex = 0;
	for (float t = 0; t < length(); t += subpointsDistance)
	{
		auto normalisedOffset = getNormalisedOffset(t);

		if (static_cast<int>(normalisedOffset) > previousIndex && keepOriginalPoints)
		{
			previousIndex = static_cast<int>(normalisedOffset);
			subsplinePath.push_back( points[previousIndex + 1]);
		}

		Point p = getSplineSubpoint(normalisedOffset);
		subsplinePath.push_back(p);
	}
	if (keepOriginalPoints)
		subsplinePath.push_back( points[size() - 2]);

	return subsplinePath;
}

std::string Timer::getString(int miliseconds)
{
	if (miliseconds < 0)
		return "--:--.--";

	std::string result;

	int minutes = miliseconds / 1000 / 60;
	int second = (miliseconds / 1000) % 60;
	int __seconds = (miliseconds / 10) % 100;

	result = std::to_string(minutes);
	result += ":";
	result += std::to_string(second + 100).substr(1, 2);
	result += ".";
	result += std::to_string(__seconds + 100).substr(1, 2);

	return result;
}