#pragma once
#include <string>

namespace AE
{
	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;

		virtual void PlaySound(int soundID, float volume) = 0;
		virtual int LoadSound(std::string path) = 0;
	};

	class NullAudio : public AudioSystem
	{
	public:
		virtual void PlaySound(int, float) override {}
		virtual int LoadSound(std::string path) { return -1; };

	};
}