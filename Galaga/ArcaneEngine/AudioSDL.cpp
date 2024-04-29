#include "AudioSDL.h"
#include <cassert>
#include <iostream>

AudioSDL::AudioSDL()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		assert(false);
	}
}

AudioSDL::~AudioSDL()
{
}

void AudioSDL::PlaySound(int soundID, float volume)
{
	Mix_VolumeMusic(static_cast<int>(volume));
	Mix_PlayChannel(-1, m_Sounds[soundID], 0);
}

void AudioSDL::StopSound(int )
{
}

void AudioSDL::StopAllSounds()
{
}

int AudioSDL::LoadSound(std::string path)
{
	auto sound{Mix_LoadWAV(("../Galaga/Resources/" + path).c_str())};
	if (!sound)
	{
		std::cout << "Sound could not be loaded at path: " << path.c_str() << "\n";
		return -1;
	}

	m_Sounds.emplace_back(sound);
	return static_cast<int>(m_Sounds.size() - 1);
}
