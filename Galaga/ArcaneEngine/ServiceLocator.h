#pragma once

#include "Audio.h"

#include <memory>

namespace AE
{
	class ServiceLocator final
	{
	public:

		static AudioSystem* GetAudio() { return m_AudioService.get(); }
		static void ProvideAudio(std::unique_ptr<AudioSystem>&& audioService);

	private:
		static std::unique_ptr<AudioSystem> m_AudioService;
	};
}
