#include "PlayList.h"

#include <ctime>
#include <algorithm>

std::unique_ptr<PlayList> PlayList::_instance;
std::vector<PlayList::PlayListData> PlayList::playlist;
int PlayList::currentSong;
int PlayList::songStartedAt;

void PlayList::Init()
{
	_instance = std::unique_ptr<PlayList>(new PlayList());

	playlist.push_back({ SoundManager::Instance()->registerSoundInstance(Sounds::playlist_01), 60000 * 2.1531833333f });
	playlist.push_back({ SoundManager::Instance()->registerSoundInstance(Sounds::playlist_02), 60000 * 2.2441166667f });

	std::random_shuffle(playlist.begin(), playlist.end());
}

std::unique_ptr<PlayList>& PlayList::Instance()
{
	return _instance;
}

void PlayList::start()
{
	currentSong = 0;
	songStartedAt = clock();
	SoundManager::Instance()->playSound(playlist[currentSong].song, 0.5f);
}

void PlayList::mute()
{
	SoundManager::Instance()->playSound(playlist[currentSong].song, 0.0f);
}

void PlayList::unmute()
{
	SoundManager::Instance()->playSound(playlist[currentSong].song, 0.5f);
}

void PlayList::update()
{
	if (songStartedAt + playlist[currentSong].time < clock())
	{
		SoundManager::Instance()->playSound(playlist[currentSong].song, 0.0f);

		currentSong = (currentSong + 1) % playlist.size();

		al_set_sample_instance_position(playlist[currentSong].song, 0);
		SoundManager::Instance()->playSound(playlist[currentSong].song, 0.5f);

		songStartedAt = clock();
	}
}