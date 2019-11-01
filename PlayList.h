#pragma once

#include <memory>
#include <vector>
#include "SoundManager.h"

#include "enum.h"

class PlayList
{
	struct PlayListData
	{
		ALLEGRO_SAMPLE_INSTANCE* song;
		float time;
	};

public:

	static void Init();
	static std::unique_ptr<PlayList>& Instance();
	static void start();
	static void mute();
	static void unmute();
	static void update();

private:


public:


private:

	static int currentSong;
	static int songStartedAt;

	static std::vector<PlayListData> playlist;

	static std::unique_ptr<PlayList> _instance;
};