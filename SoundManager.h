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

	static void Init();
	static void DeInit();
	static SoundManager* Instance();

	void readSounds();

	ALLEGRO_SAMPLE_INSTANCE* registerSoundInstance(Sounds soundName);

	void playSound(ALLEGRO_SAMPLE_INSTANCE* sample, float volume = 1.0f, float pan = 0.0f, float speed = 1.0f);
	void playSample(Sounds soundName, float volume = 1.0f, float pan = 0.0f, float speed = 1.0f);

	void setCameraPosition(const Point& newCenter, const Point& newLookAt) { cameraCenter = newCenter; cameraLookAt = newLookAt; }


private:

	SoundManager();

public:

	static ALLEGRO_VOICE *voice;
	static ALLEGRO_MIXER *mixer;

	static std::vector<ALLEGRO_MIXER*> submixers;
	static std::vector<ALLEGRO_SAMPLE_INSTANCE*> sampleInstances;

	static std::vector<SoundData> sounds;

private:

	Point cameraCenter;
	Point cameraLookAt;

	static SoundManager* _instance;
};