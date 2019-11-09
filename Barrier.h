#pragma once

#include "MapObject.h"

class Barrier : public MapObject
{
public:

	Barrier(MapObject& mapObject);

	void calculateFinalGeometry();
	void recalculateFinalGeometry();
	std::vector<std::pair<char*, char*>> getObjectXMLTags();
	std::vector<std::vector<std::pair<char*, std::string>>> getObjectXMLExtraTags();

private:

	void setTexture();

public:

	
private:
	Textures textureName;

};