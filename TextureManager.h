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
		bool doMipmapping = true;
		unsigned int idTexture;
	};

public:

	static void Init();
	static void DeInit();
	static TextureManager* Instance();

	static Textures getTextureFromFilePath(std::string filePath);

	void readTextures();
	void readLoadingTexture();
	unsigned int getLoadingTextureId() { return loadingTexture.idTexture; }

private:

	int LoadImageA(char *filename);

public:
	
	static std::vector<TextureData> textures;
	static TextureData loadingTexture;


private:

	static TextureManager* _instance;

};