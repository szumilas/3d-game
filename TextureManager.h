#pragma once

#include <vector>

#include <IL/il.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class TextureManager
{
public:

	~TextureManager();

	int readTextures();

private:

	int LoadImageA(char *filename);

public:

	std::vector<unsigned int> textureIds;

private:


	std::vector<std::string> texturePaths{
		"tex1.png",
		"tex2.png",
	};


};