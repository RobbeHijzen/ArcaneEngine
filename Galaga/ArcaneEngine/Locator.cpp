#include "Locator.h"

std::unique_ptr<Audio> Locator::m_AudioService = std::make_unique<NullAudio>();

void Locator::ProvideAudio(std::unique_ptr<Audio>&& audioService)
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

