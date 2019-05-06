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
		ALLEGRO_SAMPLE* soundData;
	};


public:
	SoundManager();
	~SoundManager();

	void readSounds();

	ALLEGRO_SAMPLE_INSTANCE* registerSoundInstance(Sounds soundName);

	void playSound(ALLEGRO_SAMPLE_INSTANCE* sample, float volume = 1.0f, float pan = 0.0f, float speed = 1.0f);

	void serCameraPosition(const Point& newCenter, const Point& newLookAt) { cameraCenter = newCenter; cameraLookAt = newLookAt; }

private:

	Point cameraCenter;
	Point cameraLookAt;

public:

	ALLEGRO_VOICE *voice;
	ALLEGRO_MIXER *mixer;

	std::vector<ALLEGRO_MIXER*> submixers;
	std::vector<ALLEGRO_SAMPLE_INSTANCE*> sampleInstances;

	std::vector<SoundData> sounds{
		{ Sounds::engine, "engine.wav" },
		{ Sounds::engine2, "engine2.wav" },
		{ Sounds::drift, "drift.wav" },
	};
};