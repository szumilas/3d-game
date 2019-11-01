#include "SoundManager.h"

#include <algorithm>

SoundManager* SoundManager::_instance;
std::vector<SoundManager::SoundData> SoundManager::sounds
{
	{ Sounds::engine, "engine.wav" },
	{ Sounds::engine2, "engine2.wav" },
	{ Sounds::drift, "drift.wav" },
	{ Sounds::crash_01, "crash01.wav" },
	{ Sounds::crash_02, "crash02.wav" },
	{ Sounds::crash_03, "crash03.wav" },
	{ Sounds::race_timer, "race_timer.wav" },
	{ Sounds::new_record, "new_record.wav" },
	{ Sounds::menu_click, "menu_click.wav" },
	{ Sounds::menu_accept, "menu_accept.wav" },
};

ALLEGRO_VOICE *SoundManager::voice;
ALLEGRO_MIXER *SoundManager::mixer;

std::vector<ALLEGRO_MIXER*> SoundManager::submixers;
std::vector<ALLEGRO_SAMPLE_INSTANCE*> SoundManager::sampleInstances;

SoundManager::SoundManager()
{
	if (!al_init())
	{
		throw Exceptions::ERR_WHILE_INIT_ALLEGRO;
	}

	al_init_acodec_addon();

	if (!al_install_audio())
	{
		throw Exceptions::ERR_WHILE_INIT_SOUND;
	}

	voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	if (!voice)
	{
		throw Exceptions::ERR_WHILE_CREATE_ALLEGRO_VOICE;
	}
	
	mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32,	ALLEGRO_CHANNEL_CONF_2);

	if (!al_attach_mixer_to_voice(mixer, voice))
	{
		throw Exceptions::ERR_WHILE_ATTACHING_MIXER_TO_VOICE;
	}

	al_reserve_samples(5);
}

void SoundManager::DeInit()
{	
	for (auto& sampleInstance : sampleInstances)
	{
		al_set_sample(sampleInstance, NULL);
		al_destroy_sample_instance(sampleInstance);
	}

	for (auto& sound : sounds)
	{
		al_destroy_sample(sound.soundData);
	}

	for (auto& submixer : submixers)
	{
		al_destroy_mixer(submixer);
	}

	al_destroy_mixer(mixer);
	al_destroy_voice(voice);

	al_uninstall_audio();

	delete(_instance);
	_instance = nullptr;
}

void SoundManager::Init()
{
	_instance = new SoundManager;
}

SoundManager* SoundManager::Instance()
{
	return _instance;
}

void SoundManager::readSounds()
{
	for (auto& sound : sounds)
	{
		std::string fullPath = "Data/Sounds/" + sound.filePath;

		sound.soundData = al_load_sample(fullPath.c_str());

		if (!sound.soundData)
		{
			throw Exceptions::ERR_WHILE_LOADING_SAMPLE;
		}
	}

	std::sort(sounds.begin(), sounds.end(), [](SoundData& a, SoundData& b) { return a.soundName < b.soundName; });
}

void SoundManager::playSound(ALLEGRO_SAMPLE_INSTANCE* sample, float volume, float pan, float speed)
{
	al_set_sample_instance_gain(sample, volume);
	al_set_sample_instance_pan(sample, pan);
	al_set_sample_instance_speed(sample, speed);
}

void SoundManager::playSample(Sounds soundName, float volume, float pan, float speed)
{
	al_play_sample(sounds[static_cast<int>(soundName)].soundData, volume, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
}

ALLEGRO_SAMPLE_INSTANCE* SoundManager::registerSoundInstance(Sounds soundName)
{
	submixers.push_back(al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2));

	if (!submixers.back())
	{
		throw Exceptions::ERR_WHILE_CREATE_ALLEGRO_MIXER;
	}

	ALLEGRO_SAMPLE_INSTANCE* newInstance = al_create_sample_instance(NULL);
	sampleInstances.push_back(newInstance);

	if (!sampleInstances.back())
	{
		throw Exceptions::ERR_WHILE_CREATE_SAMPLE;
	}
	if (!al_set_sample(sampleInstances.back(), sounds[static_cast<int>(soundName)].soundData))
	{
		throw Exceptions::ERR_WHILE_SET_SAMPLE_PTR;
	}
	if (!al_attach_sample_instance_to_mixer(sampleInstances.back(), submixers.back()))
	{
		throw Exceptions::ERR_WHILE_ATTACHING_SAMPLE_INSTANCE_TO_MIXER;
	}
	if (!al_attach_mixer_to_mixer(submixers.back(), mixer))
	{
		throw Exceptions::ERR_WHILE_ATTACHING_SUBMIXER_TO_MIXER;
	}

	al_set_sample_instance_playmode(newInstance, ALLEGRO_PLAYMODE_LOOP);
	al_play_sample_instance(newInstance);

	al_set_sample_instance_gain(newInstance, 0);

	return newInstance;
}