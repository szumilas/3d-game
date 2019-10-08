#define NOFULLSCREEN

#include "Game.h"

int main(int argc, char**agrv)
{
	Game game(argc, agrv);
	game.play();

	return 0;
}