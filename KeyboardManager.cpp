#include "KeyboardManager.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void KeyboardManager::getKeys()
{
	keys.clear();
	for (int q = 0; q < 200; q++)
		keys.push_back(GetAsyncKeyState(q));
}

bool KeyboardManager::checkKey(char k)
{
	return keys[static_cast<int>(k)];
}