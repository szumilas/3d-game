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

	static void Init();
	static void DeInit();
	static TextureManager* Instance();

	void readTextures();

private:

	int LoadImageA(char *filename);

public:
	
	static std::vector<TextureData> textures;


private:

	static TextureManager* _instance;

};