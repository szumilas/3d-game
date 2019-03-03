#pragma once

#include "MapObject.h"

class Barrier : public MapObject
{
public:

	Barrier(MapObject& mapObject) : MapObject(mapObject) {
		
		if(!_min_height) _min_height = 0.0f;
		if (colour.empty())
		{
			if (barrier == "wall") { _color.red = 170.0f / 256; _color.green = 113.0f / 256; _color.blue = 82.0f / 256; textureName = Textures::concrete_wall; _width = 0.25f;}
			else if (barrier == "kerb") { _color.red = 241.0f / 256; _color.green = 235.0f / 256; _color.blue = 240.0f / 256; textureName = Textures::concrete_wall; _width = 0.1f;}
			else if (barrier == "hedge") { _color.red = 140.0f / 256; _color.green = 195.0f / 256; _color.blue = 2.0f / 256; textureName = Textures::hedge; _width = 0.5f;}
			else if (barrier == "bollard") { _color.red = 234.0f / 256; _color.green = 207.0f / 256; _color.blue = 32.0f / 256; textureName = Textures::bollard; }
			else if (barrier == "retaining_wall") { _color.red = 213.0f / 256; _color.green = 205.0f / 256; _color.blue = 195.0f / 256; textureName = Textures::retaining_wall; _width = 0.2f;}
			else if (barrier == "guard_rail") { _color.red = 179.0f / 256; _color.green = 225.0f / 256; _color.blue = 251.0f / 256; textureName = Textures::guard_rail; }
			else if (barrier == "fence") { _color.red = 171.0f / 256; _color.green = 187.0f / 256; _color.blue = 197.0f / 256; textureName = Textures::fence; }
			else { _color.red = 128.0f / 256; _color.green = 128.0f / 256; _color.blue = 128.0f / 256; textureName = Textures::concrete_wall; }
		}
		if (height.empty())
		{
			if (barrier == "wall") { _height = 2.0f; }
			else if (barrier == "kerb") { _height = 0.1f; }
			else if (barrier == "hedge") { _height = 1.0f; }
			else if (barrier == "bollard") { _height = 0.6f; }
			else if (barrier == "retaining_wall") { _height = 1.0f; }
			else if (barrier == "guard_rail") { _height = 0.75f; }
			else if (barrier == "fence") { _height = 1.5f; }
			else { _height = 1.1f; }
		}
	};

	void calculateFinalGeometry();
	
private:
	Textures textureName;

};