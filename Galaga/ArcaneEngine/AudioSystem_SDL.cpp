#include "AudioSystem_SDL.h"
#include <cassert>
#include <iostream>

#include "AudioClip.h"
#include "SDL_mixer.h"

#include <condition_variable>
#include <mutex>
#include <queue>

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

	unsigned short CreateSoundClip(std::string path, unsigned short volume)
	{
		m_AudioClips.emplace_back(std::make_unique<AudioClip>(path, volume));

		return static_cast<unsigned short>(m_AudioClips.size() - 1);
	}

	void PlaySound(int audioClipID)
	{
		std::lock_guard<std::mutex> lock{m_Mutex};
		m_ClipQueue.push(m_AudioClips[audioClipID].get());

		m_Cv.notify_one();
	}
	
	bool LoadSound(AudioClip* audioClip)
	{
		auto sound{ Mix_LoadWAV(("../Resources/" + audioClip->GetSoundPath()).c_str()) };
		if (!sound)
		{
			std::cout << "Sound could not be loaded at path: " << audioClip->GetSoundPath().c_str() << "\n";
			return false;
		}

		m_SoundChunks.emplace_back(sound);
		audioClip->SetSoundID(static_cast<unsigned short>(m_SoundChunks.size() - 1));
		audioClip->SetLoaded();

		return true;
	}

	void StartSoundQueue()
	{
		while (m_Run)
		{
			std::unique_lock<std::mutex> lock{ m_Mutex };

			m_Cv.wait(lock, [&]{ return !m_ClipQueue.empty() || !m_Run; });
			if (!m_Run) break;

			AudioClip* audioClip{ m_ClipQueue.front()};
			m_ClipQueue.pop();

			lock.unlock();

			if (!audioClip->IsLoaded())
			{
				if (!LoadSound(audioClip))
				{
					continue;
				}
			}

			auto soundChunk{ m_SoundChunks[audioClip->GetSoundID()]};
			Mix_VolumeChunk(soundChunk, static_cast<int>(audioClip->GetVolume()));
			Mix_PlayChannel(-1, soundChunk, 0);
		}
	}

	void Stop() 
	{ 
		m_Run = false; 
		m_Cv.notify_one();
	}

private:

	std::vector<std::unique_ptr<AudioClip>> m_AudioClips{};
	std::vector<Mix_Chunk*> m_SoundChunks{};
	std::queue<AudioClip*> m_ClipQueue{};

	bool m_Run{ true };

	std::mutex m_Mutex{};
	std::condition_variable m_Cv{};
};


AudioSystem_SDL::AudioSystem_SDL()
	: m_pImpl{ std::make_unique<AudioSDLImpl>() }
{
}


void AudioSystem_SDL::PlaySound(int audioClipID)
{
	m_pImpl->PlaySound(audioClipID);
}

unsigned short AE::AudioSystem_SDL::CreateSoundClip(std::string path, unsigned short volume)
{
	return m_pImpl->CreateSoundClip(path, volume);
}

void AudioSystem_SDL::StartSoundQueue()
{
	m_pImpl->StartSoundQueue();
}

void AE::AudioSystem_SDL::Stop()
{
	m_pImpl->Stop();
}
