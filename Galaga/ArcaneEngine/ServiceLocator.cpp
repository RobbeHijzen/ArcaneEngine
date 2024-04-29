#include "ServiceLocator.h"

using namespace AE;

std::unique_ptr<AudioSystem> ServiceLocator::m_AudioService = std::make_unique<NullAudio>();

void ServiceLocator::ProvideAudio(std::unique_ptr<AudioSystem>&& audioService)
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

