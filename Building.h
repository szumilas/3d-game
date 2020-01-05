#pragma once

#include "MapObject.h"

class Building : public MapObject
{
public:

	Building(MapObject& mapObject);

	virtual void display();
	void calculateFinalGeometry();
	void recalculateFinalGeometry();
	std::vector<std::pair<char*, char*>> getObjectXMLTags();
	std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags();
	static void setCameraAngleFlag(std::pair<Point, Point>& camera);

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void applyGenericTextures();

protected:

	virtual void generateWalls();

public:

	bool roofNeeded = true;


private:

	static long cameraAngleFlag;

	bool genericWallTexture = false;
	Textures emptyWallTextureName;
	Textures smallWallTextureName;
	Textures bigWallTextureName;

	struct ConditionalPolygon
	{
		unsigned int idPolygon = 0;
		int angle;
	};

	std::vector<ConditionalPolygon> conditionalPolygons;


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
		int angle;
	};

	std::vector<Wall> walls;

};