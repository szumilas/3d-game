#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <allegro5/allegro_acodec.h>

#include "enum.h"
#include "GlobalStructures.h"

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

	void playSound(Sounds soundName, float RPM, const Point& soundPosition);

	void serCameraPosition(const Point& newCenter, const Point& newLookAt) { cameraCenter = newCenter; cameraLookAt = newLookAt; }

private:

	Point cameraCenter;
	Point cameraLookAt;

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