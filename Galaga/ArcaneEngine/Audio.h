#pragma once
#include <string>

namespace AE
{
	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;

		virtual void PlaySound(int audioClipID) = 0;
		virtual unsigned short CreateSoundClip(std::string path, unsigned short volume) = 0;

		virtual void StartSoundQueue() = 0;
		virtual void Stop() = 0;
	};

	class NullAudio : public AudioSystem
	{
	public:
		virtual void PlaySound(int) override {}
		virtual unsigned short CreateSoundClip(std::string, unsigned short) { return 0; };

		virtual void StartSoundQueue() override {};
		virtual void Stop() override {};
	};
}