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

struct node
{
	long long id;
	double lon;
	double lat;

	float posX;
	float posY;
};