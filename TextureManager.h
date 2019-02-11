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
		{ Textures::sedesowiec_front, 24.0f, 47.0f, "sedesowiec_front.jpg"},
		{ Textures::sedesowiec_side, 17.0f, 47.0f, "sedesowiec_side.jpg"},
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
		{ Textures::water, 1.0f, 1.0f, "water.jpg" },
		{ Textures::bus_shelter, 1.5f, 3.0f, "bus_shelter.png" },
		{ Textures::bus_shelter_with_sign, 1.5f, 3.0f, "bus_shelter_with_sign.png" },
		{ Textures::crossing, 5.0f, 1.0f, "crossing.png" },
		{ Textures::pasaz_grunwaldzki, 45.0f, 20.0f, "pasaz_grunwaldzki.png" },
		{ Textures::pasaz_grunwaldzki_tunnel, 45.0f, 20.0f, "pasaz_grunwaldzki_tunnel.png" },
		{ Textures::pasaz_grunwaldzki_banner, 20.0f, 20.0f, "pasaz_grunwaldzki_banner.png" },
		{ Textures::pasaz_grunwaldzki_single, 1.0f, 20.0f, "pasaz_grunwaldzki_single.png" },
		{ Textures::pasaz_grunwaldzki_front, 5.0f, 10.0f, "pasaz_grunwaldzki_front.png" },
		{ Textures::glass_elevation, 5.0f, 5.0f, "glass_elevation.jpg" },
		{ Textures::grunwaldzki, 6.0f, 3.0f, "grunwaldzki.png" },
		{ Textures::PwrC13, 10.0f, 6.0f, "PwrC13.png" },
		{ Textures::building_00_big, 15.0f, 15.0f, "building_00_big.jpg" },
		{ Textures::building_00_small, 3.0f, 15.0f, "building_00_small.jpg" },
		{ Textures::building_00_empty, 1.0f, 15.0f, "building_00_empty.jpg" },
		{ Textures::building_01_big, 9.0f, 15.0f, "building_01_big.jpg" },
		{ Textures::building_01_small, 3.0f, 15.0f, "building_01_small.jpg" },
		{ Textures::building_01_empty, 0.25f, 15.0f, "building_01_empty.jpg" },
		{ Textures::building_02_big, 9.0f, 15.0f, "building_02_big.jpg" },
		{ Textures::building_02_small, 3.0f, 15.0f, "building_02_small.jpg" },
		{ Textures::building_02_empty, 0.8f, 15.0f, "building_02_empty.jpg" },
		{ Textures::building_03_big, 21.0f, 15.0f, "building_03_big.jpg" },
		{ Textures::building_03_small, 3.0f, 15.0f, "building_03_small.jpg" },
		{ Textures::building_03_empty, 0.6f, 15.0f, "building_03_empty.jpg" },
		{ Textures::building_04_big, 10.0f, 15.0f, "building_04_big.jpg" },
		{ Textures::building_04_small, 3.0f, 15.0f, "building_04_small.jpg" },
		{ Textures::building_04_empty, 0.5f, 15.0f, "building_04_empty.jpg" },
		{ Textures::building_05_big, 24.0f, 15.0f, "building_05_big.jpg" },
		{ Textures::building_05_small, 4.0f, 15.0f, "building_05_small.jpg" },
		{ Textures::building_05_empty, 2.0f, 15.0f, "building_05_empty.jpg" },
		{ Textures::building_06_big, 18.0f, 15.0f, "building_06_big.jpg" },
		{ Textures::building_06_small, 3.0f, 15.0f, "building_06_small.jpg" },
		{ Textures::building_06_empty, 1.0f, 15.0f, "building_06_empty.jpg" },
		{ Textures::building_07_big, 12.0f, 15.0f, "building_07_big.jpg" },
		{ Textures::building_07_small, 3.0f, 15.0f, "building_07_small.jpg" },
		{ Textures::building_07_empty, 1.0f, 15.0f, "building_07_empty.jpg" },
		{ Textures::skoda_octavia, -1.0f, -1.0f, "skoda_octavia.png" },
		{ Textures::toyota_hilux, -1.0f, -1.0f, "toyota_hilux.png" },
		{ Textures::lamborghini_huracan, -1.0f, -1.0f, "lamborghini_huracan.png" },
		{ Textures::toyota_yaris, -1.0f, -1.0f, "toyota_yaris.png" },
		{ Textures::suzuki_vitara, -1.0f, -1.0f, "suzuki_vitara.png" },
		{ Textures::rolls_royce_phantom, -1.0f, -1.0f, "rolls_royce_phantom.png" },
		{ Textures::alfa_romeo_147, -1.0f, -1.0f, "alfa_romeo_147.png" },
		{ Textures::audi_r8_v10_coupe, -1.0f, -1.0f, "audi_r8_v10_coupe.png" },
		{ Textures::no_texture, 1.0f, 1.0f, "no_texture.png" },
	};


private:

};