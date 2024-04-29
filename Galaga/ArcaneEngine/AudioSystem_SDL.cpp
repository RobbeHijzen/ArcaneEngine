#include "AudioSystem_SDL.h"
#include <cassert>
#include <iostream>

#include "SDL_mixer.h"

using namespace AE;

class AudioSystem_SDL::AudioSDLImpl
{
public:

	AudioSDLImpl()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			assert(false);
		}
	}

	void PlaySound(int soundID, float volume)
	{
		Mix_VolumeMusic(static_cast<int>(volume));
		Mix_PlayChannel(-1, m_Sounds[soundID], 0);
	}
	void StopSound(int )
	{

	}
	void StopAllSounds()
	{

	}
	int LoadSound(std::string path)
	{
		auto sound{ Mix_LoadWAV(("../Resources/" + path).c_str()) };
		if (!sound)
		{
			std::cout << "Sound could not be loaded at path: " << path.c_str() << "\n";
			return -1;
		}

		m_Sounds.emplace_back(sound);
		return static_cast<int>(m_Sounds.size() - 1);
	}

private:

	std::vector<Mix_Chunk*> m_Sounds{};
};


AudioSystem_SDL::AudioSystem_SDL()
	: m_pImpl{ std::make_unique<AudioSDLImpl>() }
{
}

AudioSystem_SDL::~AudioSystem_SDL()
{
}

void AudioSystem_SDL::PlaySound(int soundID, float volume)
{
	m_pImpl->PlaySound(soundID, volume);
}

void AudioSystem_SDL::StopSound(int soundID)
{
	m_pImpl->StopSound(soundID);
}

void AudioSystem_SDL::StopAllSounds()
{
	m_pImpl->StopAllSounds();
}

int AudioSystem_SDL::LoadSound(std::string path)
{
	return m_pImpl->LoadSound(path);
}
