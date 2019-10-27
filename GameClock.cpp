#include "GameClock.h"

#include <ctime>

std::unique_ptr<GameClock> GameClock::_instance;
int GameClock::pausedAt = 0;
bool GameClock::paused = false;
int GameClock::sumOfPauseTime = 0;

void GameClock::Init()
{
	_instance = std::unique_ptr<GameClock>(new GameClock());
}

std::unique_ptr<GameClock>& GameClock::Instance()
{
	return _instance;
}

int GameClock::getClock()
{
	if (paused)
		return pausedAt - sumOfPauseTime;
	else
		return clock() - sumOfPauseTime;
}

void GameClock::pause()
{
	paused = true;
	pausedAt = clock();
}

void GameClock::resume()
{
	paused = false;
	sumOfPauseTime += clock() - pausedAt;
}

void GameClock::clear()
{
	paused = false;
	sumOfPauseTime = 0;
	pausedAt = 0;
}