#pragma once
#include <string>

namespace AE
{
	enum class SoundType
	{
		Chunk,
		Music
	};

	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;

		virtual void PlaySound(int audioClipID) = 0;
		virtual unsigned short CreateSoundClip(std::string path, unsigned short volume, SoundType type = SoundType::Chunk) = 0;

		virtual void StartSoundQueue() = 0;
		virtual void Stop() = 0;
		virtual void ToggleMute() {}
	};

	class NullAudio : public AudioSystem
	{
	public:
		virtual void PlaySound(int) override {}
		virtual unsigned short CreateSoundClip(std::string, unsigned short, SoundType = SoundType::Chunk) override { return 0; };

		virtual void StartSoundQueue() override {};
		virtual void Stop() override {};
	};
}