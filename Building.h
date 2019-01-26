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

protected:

	virtual void generateWalls();

public:


private:


protected:

	struct Wall
	{
		Point p1;
		Point p2;
		Color color;
		unsigned int idTexture;
		Textures textureName;
		float wallLenght;
		float xRatio = 0.0f;
		float yRatio = 0.0f;
	};

	std::vector<Wall> walls;

};