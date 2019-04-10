#include "SoundManager.h"

#include <algorithm>

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
}

SoundManager::~SoundManager()
{	
	for (auto& sample : samples)
	{
		al_set_sample(sample, NULL);
		al_destroy_sample_instance(sample);
	}

	for (auto& sample_data : samples_data)
	{
		al_destroy_sample(sample_data);
	}

	for (auto& submixer : submixers)
	{
		al_destroy_mixer(submixer);
	}

	al_destroy_mixer(mixer);
	al_destroy_voice(voice);

	al_uninstall_audio();
}

void SoundManager::readSounds()
{
	for (auto& sound : sounds)
	{
		submixers.push_back(al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2));

		if (!submixers.back())
		{
			throw Exceptions::ERR_WHILE_CREATE_ALLEGRO_MIXER;
		}

		std::string fullPath = "Data/Sounds/" + sound.filePath;

		samples_data.push_back(al_load_sample(fullPath.c_str()));

		if (!samples_data.back())
		{
			throw Exceptions::ERR_WHILE_LOADING_SAMPLE;
		}

		samples.push_back(al_create_sample_instance(NULL));

		if (!samples.back())
		{
			throw Exceptions::ERR_WHILE_CREATE_SAMPLE;
		}
		if (!al_set_sample(samples.back(), samples_data.back()))
		{
			throw Exceptions::ERR_WHILE_SET_SAMPLE_PTR;
		}
		if (!al_attach_sample_instance_to_mixer(samples.back(), submixers.back()))
		{
			throw Exceptions::ERR_WHILE_ATTACHING_SAMPLE_INSTANCE_TO_MIXER;
		}
		if (!al_attach_mixer_to_mixer(submixers.back(), mixer))
		{
			throw Exceptions::ERR_WHILE_ATTACHING_SUBMIXER_TO_MIXER;
		}
	}

	std::sort(sounds.begin(), sounds.end(), [](SoundData& a, SoundData& b) { return a.soundName < b.soundName; });

	al_set_sample_instance_playmode(samples[0], ALLEGRO_PLAYMODE_LOOP);
	al_play_sample_instance(samples[0]);
	al_set_sample_instance_gain(samples[0], 1);
}

void SoundManager::playSound(Sounds soundName, float RPM, const Point& soundPosition)
{
	if (RPM >= 6000)
		RPM = 5999;

	float speed = (RPM / 3000) + 0.6;

	speed = max(speed, 1.0);

	float volume = sqrt(1 / (max(1, soundPosition.distance2D(cameraCenter))));

	vector2D lookingLine(cameraCenter, cameraLookAt);
	vector2D objectLine(cameraCenter, soundPosition);

	float angle = vector2D::directedAngle(lookingLine, objectLine);

	float finalPan = 0.0f;

	if (angle >= 0.0f && angle < PI / 2)
		finalPan = -angle / (PI / 2);
	else if (angle >= PI / 2 && angle < PI)
		finalPan = -(PI - angle) / (PI / 2);
	else if (angle >= PI && angle < 3 / 2 * PI)
		finalPan = (angle - PI) / (PI / 2);
	else if (angle >= 3 / 2 * PI && angle < 2 * PI)
		finalPan = (2 * PI - angle) / (PI / 2);

	al_set_sample_instance_pan(samples[0], finalPan);
	al_set_sample_instance_gain(samples[0], volume);
	al_set_sample_instance_speed(samples[0], speed);
}