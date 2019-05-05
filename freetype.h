#pragma once

#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL Headers 
#include <windows.h>            //(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will correctly
//catch any exceptions that we throw.
#include <stdexcept>


//MSVC will spit out all sorts of useless warnings if
//you create vectors of strings, this pragma gets rid of them.
#pragma warning(disable: 4786)

///Wrap everything in a namespace, that way can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.

#include "enum.h"
#include "GlobalStructures.h"

namespace freetype {

	void pop_projection_matrix();
	void pushScreenCoordinateMatrix();

	//Inside of this namespace, give ourselves the ability
	//to write just "vector" instead of "std::vector"
	using std::vector;

	//Ditto for string.
	using std::string;

	//This holds all of the information related to any
	//freetype font that we want to create.  
	struct font_data {
		float h;                        ///< Holds the height of the font.
		GLuint * textures;      ///< Holds the texture id's 
		GLuint list_base;             ///< Holds the first display list id

									  ///The init function will create a font of
									  ///of the height h from the file fname.
		void init(const char * fname, unsigned int h);

		///Free all the resources associated with the font.
		void clean();
	};

	///The flagship function of the library - this thing will print
	///out text at window coordinates x,y, using the font ft_font.
	///The current modelview matrix will also be applied to the text. 
	template <typename T>
	void print(const font_data &ft_font, float x, float y, const char *fmt, T args...)
	{
		// We want a coordinate system where distance is measured in window pixels.
		pushScreenCoordinateMatrix();

		display(ft_font, x, y, fmt, args);

		pop_projection_matrix();
	}

	void display(ColorName colorName, const font_data &ft_font, float x, float y, const char *fmt, ...);

	static void setGLcolor(ColorName colorName)
	{
		Color color = Color(colorName);
		glColor3f(color.red, color.green, color.blue);
	}

}  //close the namespace

#endif
