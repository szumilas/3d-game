#pragma once

#include <vector>
#include <map>

#include <IL/il.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "enum.h"

class TextureManager
{
	struct TextureData
	{
		Textures textureName;
		float realWidth;
		float realHeight;
		std::string filePath;
		unsigned int idTexture;
	};

public:

	~TextureManager();

	void readTextures();

private:

	int LoadImageA(char *filename);

public:
	
	std::vector<TextureData> textures {
		{ Textures::roof, 5.0f, 3.3f, "roof.jpg"},
		{ Textures::roof_asphalt, 4.0f, 4.0f, "roof_asphalt.jpg"},
		{ Textures::tenement_house_windows, 5.0f, 15.0f, "kamienica0.jpg"},
		{ Textures::tenement_house_no_windows, 1.0f, 15.0f, "kamienica1.jpg"},
		{ Textures::apartment_windows, 20.0f, 15.0f, "apartment.jpg"},
		{ Textures::apartment_no_windows, 2.0f, 15.0f, "apartment0.jpg"},
		{ Textures::apartment_one_window, 2.0f, 15.0f, "apartment2.jpg"},
		{ Textures::office_windows, 20.0f, 20.0f, "office.jpg"},
		{ Textures::manhattan, 24.0f, 30.0f, "manhattan.jpg"},
		{ Textures::asphalt, 1.25f, 0.972f, "asphalt.jpg"},
		{ Textures::grass, 1.25f, 0.972f, "grass.jpg"},
		{ Textures::paving, 0.0f, 0.0f, "paving.jpg"},
		{ Textures::skybox, 4000.0f, 4000.0f, "skybox.png"},
		{ Textures::sett, -1.0f, -1.0f, "sett.jpg"},
		{ Textures::rail, -1.0f, -1.0f, "tram_rail.png"},
		{ Textures::bollard, 1.0f, 0.5f, "bollard.png" },
		{ Textures::concrete_wall, 1.0f, 1.0f, "concrete_wall.jpg" },
		{ Textures::fence, 1.83f, 1.1f, "fence_with_post.png" },
		{ Textures::guard_rail, 1.5f, 0.3f, "guard_rail.png" },
		{ Textures::hedge, 2.0f, 1.0f, "hedge.png" },
		{ Textures::retaining_wall, 2.0f, 1.0f, "retaining_wall.jpg" },
	};


private:

};