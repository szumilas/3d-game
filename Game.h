#pragma once

#include <IL/il.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <time.h>

#include "GlobalVariables.h"

#include "enum.h"

#include "Object3D.h"
#include "Car.h"
#include "Orbit.h"
#include "MapManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "CameraManager.h"
#include "MapContainer.h"
#include "CarGauge.h"
#include "Screen2D.h"
#include "Menu.h"

#include "carDB.h"
#include "KeyboardManager.h"


class Game
{
public:

	enum State
	{
		mainMenu,
		race,
		freeRide,
		pause,
		mapEditor,
	};

	enum Key
	{
		ENTER = 13,
	};

public:

	Game(int argc, char**agrv);

	void play();

private:

	void init();
	static void display();
	static void displayWorld();
	static void reshape(int width, int height);
	static void timer(int);
	static void keyboard(unsigned char key, int x, int y);
	static void mouse(int button, int state, int x, int y);
	static void SpecialKeys(int key, int x, int y);
	static void look(int x, int y);
	static void SpecialKeysUp(int key, int x, int y);
	static void Update();
	static bool checkKey(Key key);
	static void displayLoadingTexture();

	static void handleMenuResponse();

public:


private:

	static int current_time;
	static int previos_time;
	static int noOfFrames;
	 
	static int windowWidth;
	static int windowHeight;
	 
	static int windowRealWidth;
	static int windowRealHeight;
	 
	static float angle;

	static int mouseXPos;
	static int mouseYPos;

	static Menu menu;
	static State gameState;
	static State previousGameState;
};