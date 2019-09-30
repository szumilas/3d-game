#pragma once

#include "GlobalStructures.h"

class ObjectWithCamera
{

public:

	virtual Point getCameraCenter();
	virtual Point getCameraLookAt();

};