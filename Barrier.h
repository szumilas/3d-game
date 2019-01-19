#pragma once

#include "MapObject.h"

class Barrier : public MapObject
{
public:

	Barrier(MapObject& mapObject) : MapObject(mapObject) {
		
		if(!_min_height) _min_height = 0.0f;
		if (colour.empty())
		{
			if (barrier == "wall") { _red = 170.0f / 256; _green = 113.0f / 256; _blue = 82.0f / 256; textureName = Textures::concrete_wall; }
			else if (barrier == "kerb") { _red = 241.0f / 256; _green = 235.0f / 256; _blue = 240.0f / 256; textureName = Textures::concrete_wall; }
			else if (barrier == "hedge") { _red = 140.0f / 256; _green = 195.0f / 256; _blue = 2.0f / 256; textureName = Textures::hedge; _width = 0.5f;}
			else if (barrier == "bollard") { _red = 234.0f / 256; _green = 207.0f / 256; _blue = 32.0f / 256; textureName = Textures::bollard; }
			else if (barrier == "retaining_wall") { _red = 213.0f / 256; _green = 205.0f / 256; _blue = 195.0f / 256; textureName = Textures::retaining_wall; }
			else if (barrier == "guard_rail") { _red = 179.0f / 256; _green = 225.0f / 256; _blue = 251.0f / 256; textureName = Textures::guard_rail; }
			else if (barrier == "fence") { _red = 171.0f / 256; _green = 187.0f / 256; _blue = 197.0f / 256; textureName = Textures::fence; }
			else { _red = 128.0f / 256; _green = 128.0f / 256; _blue = 128.0f / 256; textureName = Textures::concrete_wall; }
		}
		if (height.empty())
		{
			if (barrier == "wall") { _height = 2.0f; }
			else if (barrier == "kerb") { _height = 0.15f; }
			else if (barrier == "hedge") { _height = 1.0f; }
			else if (barrier == "bollard") { _height = 0.6f; }
			else if (barrier == "retaining_wall") { _height = 1.0f; }
			else if (barrier == "guard_rail") { _height = 0.75f; }
			else if (barrier == "fence") { _height = 1.5f; }
			else { _height = 1.1f; }
		}
	};

	void calculateFinalGeometry(TextureManager* textureManager);
	
private:
	Textures textureName;

};