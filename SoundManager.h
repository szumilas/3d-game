#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <allegro5/allegro_acodec.h>

#include "enum.h"

class SoundManager
{
	//struct SoundData
	//{
	//	Sounds soundName;
	//	std::string filePath;
	//	ALLEGRO_SAMPLE* sample = nullptr;
	//};


public:
	SoundManager();
	~SoundManager();

	void playSound(Sounds soundName, float RPM);

private:

	int diff;

public:
	//std::vector<SoundData> sounds;


	//ALLEGRO_MIXER *submixer;
	//ALLEGRO_SAMPLE_INSTANCE *sample = al_create_sample_instance(NULL);

	float speed = 1.01;

	int voicesNo;

	ALLEGRO_VOICE *voice;
	ALLEGRO_MIXER *mixer;
	//ALLEGRO_MIXER *submixer[11];
	//ALLEGRO_SAMPLE_INSTANCE *sample[11];
	//ALLEGRO_SAMPLE *sample_data[11];

	std::vector<ALLEGRO_MIXER*> submixers;
	std::vector<ALLEGRO_SAMPLE_INSTANCE*> samples;
	std::vector<ALLEGRO_SAMPLE*> samples_data;

	float sample_time;
	float max_sample_time;
};