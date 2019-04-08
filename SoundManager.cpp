#include "SoundManager.h"

SoundManager::SoundManager()
{
	voicesNo = 1;

	int maxRPM = 6000;
	int minRPM = 1000;

	submixers.resize(voicesNo);
	samples.resize(voicesNo);
	samples_data.resize(voicesNo);

	//sounds = std::vector<SoundData>
	//{
	//	{ Sounds::engine, "Data/Sounds/Eng2_03000 (2).wav" },
	//
	//};

	//if (!al_init()) {
	//	fprintf(stderr, "failed to initialize allegro!\n");
	//	//return -1;
	//}
	//
	//if (!al_install_audio()) {
	//	fprintf(stderr, "failed to initialize audio!\n");
	//	//return -1;
	//}
	//
	//if (!al_init_acodec_addon()) {
	//	fprintf(stderr, "failed to initialize audio codecs!\n");
	//	//return -1;
	//}
	//
	//if (!al_reserve_samples(1)) {
	//	fprintf(stderr, "failed to reserve samples!\n");
	//	//return -1;
	//}
	//
	//for (auto& sound : sounds)
	//{
	//	sound.sample = al_load_sample(sound.filePath.c_str());
	//
	//	if (!sound.sample) {
	//		printf("Audio clip sample not loaded!\n");
	//		//return -1; 
	//	}
	//}


	/*sample = al_load_sample("Data/Sounds/engine.wav");

	if (!sample) {
	printf("Audio clip sample not loaded!\n");
	//return -1;
	}*/

	//al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

	/*al_set_sample(sample, sounds[0].sample);
	submixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

	al_attach_sample_instance_to_mixer(sample, submixer);

	al_attach_mixer_to_mixer(submixer[i], mixer);

	al_set_sample_instance_playmode(sample, ALLEGRO_PLAYMODE_LOOP);
	al_play_sample_instance(sample);*/

	int i;

	if (!al_init()) {
		//abort_example("Could not init Allegro.\n");
	}


	al_init_acodec_addon();

	if (!al_install_audio()) {
		//abort_example("Could not init sound!\n");
	}

	voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16,
		ALLEGRO_CHANNEL_CONF_2);
	if (!voice) {
		//abort_example("Could not create ALLEGRO_VOICE.\n");
	}


	mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32,
		ALLEGRO_CHANNEL_CONF_2);

	for (auto& submixer : submixers)
	{
		submixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	}

	//if (!mixer || !submixer[0] || !submixer[1] || !submixer[2] || !submixer[3] || !submixer[4]) {
		//abort_example("al_create_mixer failed.\n");
	//}

	if (!al_attach_mixer_to_voice(mixer, voice)) {
		//abort_example("al_attach_mixer_to_voice failed.\n");
	}

	//diff = (maxRPM - minRPM) / (voicesNo - 1);

	for (int q = 0; q < voicesNo; q++)
	{
		std::string directory = "Data/Sounds/";
		std::string extension = ".wav";

		std::string RPMvalue = std::to_string(1000 + diff * q);

		std::string fullPath = directory + "1000" + "_rpm" + extension;

		fullPath = directory + "engine05" + extension;

		samples_data[q] = al_load_sample(fullPath.c_str());
	}

	for (i = 0; i < voicesNo; i++) {
		//const char *filename = argv[i + 1];
		
		if (!samples_data[i]) {
			//abort_example("Could not load sample from '%s'!\n", filename);
		}
		samples[i] = al_create_sample_instance(NULL);
		if (!samples[i]) {
			//abort_example("al_create_sample failed.\n");
		}
		if (!al_set_sample(samples[i], samples_data[i])) {
			//abort_example("al_set_sample_ptr failed.\n");
		}
		if (!al_attach_sample_instance_to_mixer(samples[i], submixers[i])) {
			//abort_example("al_attach_sample_instance_to_mixer failed.\n");
		}
		if (!al_attach_mixer_to_mixer(submixers[i], mixer)) {
			//abort_example("al_attach_mixer_to_mixer failed.\n");
		}
	}

	/* Play sample in looping mode. */
	for (i = 0; i < voicesNo; i++) {
		al_set_sample_instance_playmode(samples[i], ALLEGRO_PLAYMODE_LOOP);
		al_play_sample_instance(samples[i]);
		al_set_sample_instance_gain(samples[i], 0);
	}

	al_set_sample_instance_gain(samples[0], 1);

	//max_sample_time = al_get_sample_instance_time(sample[0]);
	//sample_time = al_get_sample_instance_time(sample[1]);
	//if (sample_time > max_sample_time)
	//	max_sample_time = sample_time;

	//log_printf("Playing...");

	//al_rest(max_sample_time);
	//
	//al_set_sample_instance_gain(sample[0], 0.5);
	//al_rest(max_sample_time);
	//
	//al_set_sample_instance_gain(sample[1], 0.25);
	//al_rest(max_sample_time);

	//al_stop_sample_instance(sample[0]);
	//al_stop_sample_instance(sample[1]);
	//log_printf("Done\n");

	



}

SoundManager::~SoundManager()
{
	//for (auto& sound : sounds)
	//{
	//	al_destroy_sample(sound.sample);
	//}

	int i;

	/* Free the memory allocated. */
	for (i = 0; i < voicesNo; i++) {
		al_set_sample(samples[i], NULL);
		al_destroy_sample(samples_data[i]);
		al_destroy_sample_instance(samples[i]);
		al_destroy_mixer(submixers[i]);
	}
	al_destroy_mixer(mixer);
	al_destroy_voice(voice);

	al_uninstall_audio();
}

void SoundManager::playSound(Sounds soundName, float RPM)
{
	if (RPM >= 6000)
		RPM = 5999;

	//for (int i = 0; i < voicesNo; i++)
	//{
	//	al_set_sample_instance_gain(samples[i], 0);
	//	al_set_sample_instance_speed(samples[i], speed);
	//}

	//if(speed > 1.0)
	//	speed += 0.01;
	//if (speed >= 2.0)
	//{
	//	speed = 1.01f;
	//	al_set_sample_instance_speed(samples[0], speed);
	//}
	

	//int first = static_cast<int>((static_cast<float>(RPM) - 1000) / diff);
	//first = max(0, first);
	//int second = first + 1;
	//second = min(second, voicesNo - 1);
	//
	//float secondVolume = (static_cast<float>(RPM) / diff - (RPM / diff));
	//int firstVolume = 1 - secondVolume;
	//
	//al_set_sample_instance_gain(samples[first], firstVolume);
	//al_set_sample_instance_gain(samples[second], secondVolume);

	//al_set_sample_instance_gain(samples[0], 1);

	speed = (RPM / 3000) + 0.6;

	speed = max(speed, 1.0);

	//if(speed > 1.0)

	//max_sample_time = al_get_sample_instance_time(samples[0]);
	//sample_time = al_get_sample_instance_time(samples[0]);
	//if (sample_time > max_sample_time)
	//	max_sample_time = sample_time;
	//
	//
	//al_rest(max_sample_time);
	



	al_set_sample_instance_speed(samples[0], speed);



	//al_rest(max_sample_time);

	/*if (RPM < 1500)
	{
		al_set_sample_instance_gain(samples[0], (1500 - RPM) / 500);
		al_set_sample_instance_gain(samples[1], 1 - (1500 - RPM) / 500);
	}
	else if (RPM >= 1500 && RPM < 2000)
	{
		al_set_sample_instance_gain(samples[1], (2000 - RPM) / 500);
		al_set_sample_instance_gain(samples[2], 1 - (2000 - RPM) / 500);
	}
	else if (RPM >= 2000 && RPM < 2500)
	{
		al_set_sample_instance_gain(samples[2], (2500 - RPM) / 500);
		al_set_sample_instance_gain(samples[3], 1 - (2500 - RPM) / 500);
	}
	else if (RPM >= 2500 && RPM < 3000)
	{
		al_set_sample_instance_gain(samples[3], (3000 - RPM) / 500);
		al_set_sample_instance_gain(samples[4], 1 - (3000 - RPM) / 500);
	}
	else if (RPM >= 3000 && RPM < 3500)
	{
		al_set_sample_instance_gain(samples[4], (3500 - RPM) / 500);
		al_set_sample_instance_gain(samples[5], 1 - (3500 - RPM) / 500);
	}
	else if (RPM >= 3500 && RPM < 4000)
	{
		al_set_sample_instance_gain(samples[5], (4000 - RPM) / 500);
		al_set_sample_instance_gain(samples[6], 1 - (4000 - RPM) / 500);
	}
	else if (RPM >= 4000 && RPM < 4500)
	{
		al_set_sample_instance_gain(samples[6], (4500 - RPM) / 500);
		al_set_sample_instance_gain(samples[7], 1 - (4500 - RPM) / 500);
	}
	else if (RPM >= 4500 && RPM < 5000)
	{
		al_set_sample_instance_gain(samples[7], (5000 - RPM) / 500);
		al_set_sample_instance_gain(samples[8], 1 - (5000 - RPM) / 500);
	}
	else if (RPM >= 5000 && RPM < 5500)
	{
		al_set_sample_instance_gain(samples[8], (5500 - RPM) / 500);
		al_set_sample_instance_gain(samples[9], 1 - (5500 - RPM) / 500);
	}
	else if (RPM >= 5500 && RPM < 6000)
	{
		al_set_sample_instance_gain(samples[9], (6000 - RPM) / 500);
		al_set_sample_instance_gain(samples[10], 1 - (6000 - RPM) / 500);
	}
	else
	{
		al_set_sample_instance_gain(samples[10], 1);
	}*/


	//al_play_sample(sounds[static_cast<int>(soundName)].sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	//al_set_sample_instance_gain(sample[0], volume);
	//al_set_sample_instance_gain(sample[1], 1 - volume);
}