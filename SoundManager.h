#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <allegro5/allegro_acodec.h>

#include "enum.h"

class SoundManager
{
	struct SoundData
	{
		Sounds soundName;
		std::string filePath;
	};


public:
	SoundManager();
	~SoundManager();

	void readSounds();

	void playSound(Sounds soundName, float RPM);

private:
	

public:

	ALLEGRO_VOICE *voice;
	ALLEGRO_MIXER *mixer;

	std::vector<ALLEGRO_MIXER*> submixers;
	std::vector<ALLEGRO_SAMPLE_INSTANCE*> samples;
	std::vector<ALLEGRO_SAMPLE*> samples_data;

	std::vector<SoundData> sounds{
		{ Sounds::engine, "engine.wav" }
	};
};