#define NOMIPMAPPING

#include "TextureManager.h"
#include <algorithm>

TextureManager* TextureManager::_instance;

std::vector<TextureManager::TextureData> TextureManager::textures
{
	{ Textures::roof, 5.0f, 3.3f, "roof.jpg" },
	{ Textures::roof_asphalt, 4.0f, 4.0f, "roof_asphalt.jpg" },
	{ Textures::tenement_house_windows, 5.0f, 15.0f, "kamienica0.jpg" },
	{ Textures::tenement_house_no_windows, 1.0f, 15.0f, "kamienica1.jpg" },
	{ Textures::apartment_windows, 20.0f, 15.0f, "apartment.jpg" },
	{ Textures::apartment_no_windows, 2.0f, 15.0f, "apartment0.jpg" },
	{ Textures::apartment_one_window, 2.0f, 15.0f, "apartment2.jpg" },
	{ Textures::office_windows, 20.0f, 20.0f, "office.jpg" },
	{ Textures::sedesowiec_front, 24.0f, 47.0f, "sedesowiec_front.jpg" },
	{ Textures::sedesowiec_side, 17.0f, 47.0f, "sedesowiec_side.jpg" },
	{ Textures::asphalt, 1.25f, 0.972f, "asphalt.jpg" },
	{ Textures::grass, 1.25f, 0.972f, "grass.jpg" },
	{ Textures::paving, 0.0f, 0.0f, "paving.jpg" },
	{ Textures::skybox, 4000.0f, 4000.0f, "skybox.png" },
	{ Textures::sett, -1.0f, -1.0f, "sett.jpg" },
	{ Textures::rail, -1.0f, -1.0f, "tram_rail.png" },
	{ Textures::bollard, 1.0f, 0.5f, "bollard.png" },
	{ Textures::concrete_wall, 1.0f, 1.0f, "concrete_wall.jpg" },
	{ Textures::fence, 1.83f, 1.1f, "fence_with_post.png", false },
	{ Textures::guard_rail, 1.5f, 0.3f, "guard_rail.png" },
	{ Textures::hedge, 2.0f, 1.0f, "hedge.png" },
	{ Textures::retaining_wall, 2.0f, 1.0f, "retaining_wall.jpg" },
	{ Textures::water, 1.0f, 1.0f, "water.jpg" },
	{ Textures::bus_shelter, 1.5f, 3.0f, "bus_shelter.png" },
	{ Textures::bus_shelter_with_sign, 1.5f, 3.0f, "bus_shelter_with_sign.png" },
	{ Textures::crossing, 5.0f, 1.0f, "crossing.png", false },
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
	{ Textures::subaru_brz, -1.0f, -1.0f, "subaru_brz.png" },
	{ Textures::rolls_royce_phantom, -1.0f, -1.0f, "rolls_royce_phantom.png" },
	{ Textures::alfa_romeo_147, -1.0f, -1.0f, "alfa_romeo_147.png" },
	{ Textures::audi_r8_v10_coupe, -1.0f, -1.0f, "audi_r8_v10_coupe.png" },
	{ Textures::car_gauge, -1.0f, -1.0f, "car_gauge.png", false },
	{ Textures::car_gauge_330, -1.0f, -1.0f, "car_gauge_330.png", false },
	{ Textures::elm_tree, -1.0f, -1.0f, "elm_tree.png" },
	{ Textures::no_texture, 1.0f, 1.0f, "no_texture.png" },
	{ Textures::map_editor_panel, 1.0f, 1.0f, "map_editor_panel.png" },
	{ Textures::counter, 1.0f, 1.0f, "counter.png", false },
	{ Textures::meta, 0.972f, 0.972f, "meta.png", false },
	{ Textures::race_barrier, 2.0f, 1.2f, "race_barrier.png", false },
	{ Textures::menu_background, -1.0f, -1.0f, "menu_background.png" },
	{ Textures::menu_arrow, -1.0f, -1.0f, "menu_arrow.png", false },
	{ Textures::wroclaw_map, -1.0f, -1.0f, "wroclaw_map.png", false },
	{ Textures::suzuki_baleno, -1.0f, -1.0f, "suzuki_baleno.png" },
	{ Textures::honda_jazz, -1.0f, -1.0f, "honda_jazz.png" },
	{ Textures::peugeot_208, -1.0f, -1.0f, "peugeot_208.png" },
	{ Textures::citroen_c3, -1.0f, -1.0f, "citroen_c3.png" },
	{ Textures::opel_corsa, -1.0f, -1.0f, "opel_corsa.png" },
	{ Textures::bmw_3, -1.0f, -1.0f, "bmw_3.png" },
	{ Textures::volkswagen_golf, -1.0f, -1.0f, "volkswagen_golf.png" },
	{ Textures::mercedes_e_200, -1.0f, -1.0f, "mercedes_e_200.png" },
	{ Textures::mitsubishi_pajero, -1.0f, -1.0f, "mitsubishi_pajero.png" },
	{ Textures::mazda_6, -1.0f, -1.0f, "mazda_6.png" },
	{ Textures::mini_cooper, -1.0f, -1.0f, "mini_cooper.png" },
	{ Textures::smart_fortwo, -1.0f, -1.0f, "smart_fortwo.png" },
	{ Textures::jaguar_s_type, -1.0f, -1.0f, "jaguar_s_type.png" },
	{ Textures::audi_a4, -1.0f, -1.0f, "audi_a4.png" },
	{ Textures::honda_cr_z, -1.0f, -1.0f, "honda_cr_z.png" },
	{ Textures::logo, -1.0f, -1.0f, "logo.png", false },
};

void TextureManager::DeInit()
{
	for (auto& texture : textures)
		glDeleteTextures(1, &texture.idTexture);
}

void TextureManager::Init()
{
	_instance = new TextureManager;
}

TextureManager* TextureManager::Instance()
{
	return _instance;
}

void TextureManager::readTextures()
{
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	bool mipmappingAllowed = false;
#ifdef MIPMAPPING
	mipmappingAllowed = true;
#endif

	for (auto& texture : textures)
	{
		int image = LoadImageA(const_cast<char*>(("Data/Textures/" + texture.filePath).c_str()));
		if (image == -1)
		{
			throw Exceptions::ERR_WHILE_LOADING_IMAGE;
		}
		
		auto textureIdCreated = static_cast<unsigned int>(image);
		
		/* OpenGL texture binding of the image loaded by DevIL  */
		glGenTextures(1, &textureIdCreated); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, textureIdCreated); /* Binding of texture name */
		
		if (texture.doMipmapping && mipmappingAllowed)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); /* We will use linear interpolation for minifying filter */

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
				0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		}
		
		
		texture.idTexture = textureIdCreated;
		//textureIds.({ texturePath.first.textureName, { texturePath.first.textureName, texturePath.first.realWidth, texturePath.first.realHeight, textureName} });
		ilDeleteImages(1, &textureIdCreated); /* Because we have already copied image data into texture data we can release memory used by image. */
	
	}

	std::sort(textures.begin(), textures.end(), [](TextureData& a, TextureData& b) { return a.textureName < b.textureName; });

}

int TextureManager::LoadImageA(char *filename)
{
	ILboolean success;
	ILuint image;

	ilGenImages(1, &image); /* Generation of one image name */
	ilBindImage(image); /* Binding of image name */
	success = ilLoadImage(filename); /* Loading of the image filename by DevIL */

	if (success) /* If no error occured: */
	{
		/* Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success)
		{
			throw Exceptions::ERR_WHILE_CONVERTING_IMAGE;
		}
	}
	else
	{
		throw Exceptions::ERR_WHILE_LOADING_IMAGE;
	}

	return image;
}