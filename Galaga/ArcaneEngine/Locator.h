#pragma once

#include "Audio.h"

#include <memory>

class Locator final
{
public:

	static Audio* GetAudio() { return m_AudioService.get(); }
	static void ProvideAudio(std::unique_ptr<Audio>&& audioService)
	{
		if (!audioService)
		{
			m_AudioService = std::make_unique<NullAudio>();
		}
		else
		{
			m_AudioService = std::move(audioService);
		}
	}

private:
	static std::unique_ptr<Audio> m_AudioService;
};

