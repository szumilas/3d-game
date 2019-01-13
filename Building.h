#pragma once

#include "MapObject.h"

class Building : public MapObject
{
public:

	Building(MapObject& mapObject);

	//void display();
	void calculateFinalGeometry(TextureManager* textureManager);

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void generateWalls();

public:


private:

	struct Wall
	{
		Point p1;
		Point p2;
		Color color;
		unsigned int idTexture;
		float wallLenght;
		float xRatio;
		float yRatio;
	};

	std::vector<Wall> walls;

};