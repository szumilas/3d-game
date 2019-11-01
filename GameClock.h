#pragma once

#include <memory>

class GameClock
{
public:

	static void Init();
	static std::unique_ptr<GameClock>& Instance();
	static void pause();
	static void resume();
	static void clear();

	static int getClock();
	static bool isPause() { return paused; }

private:


public:


private:

	static int pausedAt;
	static bool paused;

	static int sumOfPauseTime;

	static std::unique_ptr<GameClock> _instance;
};