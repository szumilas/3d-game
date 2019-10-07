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

#include "carDB.h"
#include "KeyboardManager.h"


class Game
{
public:

	Game(int argc, char**agrv);

};