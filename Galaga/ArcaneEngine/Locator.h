#pragma once

#include "Audio.h"

#include <memory>

class Locator final
{
public:

	static Audio* GetAudio() { return m_AudioService.get(); }
	static void ProvideAudio(std::unique_ptr<Audio>&& audioService);
	
private:
	static std::unique_ptr<Audio> m_AudioService;
};

