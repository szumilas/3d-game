#include "TextureManager.h"

TextureManager::~TextureManager()
{
	for (auto& textureId : textureIds)
		glDeleteTextures(1, &textureId);
}

int TextureManager::readTextures()
{
	for (auto& texturePath : texturePaths)
	{

		int image = LoadImageA(const_cast<char*>(("Data/Textures/" + texturePath).c_str()));
		if (image == -1)
		{
			return 0;
		}

		auto textureName = static_cast<unsigned int>(image);

		/* OpenGL texture binding of the image loaded by DevIL  */
		glGenTextures(1, &textureName); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, textureName); /* Binding of texture name */


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */


		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
			0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		textureIds.push_back(textureName);
		ilDeleteImages(1, &textureName); /* Because we have already copied image data into texture data we can release memory used by image. */
								   //	
	}

	return 1;
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
			return -1;
		}
	}
	else
		return -1;

	return image;
}